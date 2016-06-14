#include "Hiro.h"
#include "SphereWorld.h"
#include "UI.h"
#include "Camera.h"
#include "Pellet.h"
//#include "SDK_callbacks.h"

#define POWER 50

Hiro::Hiro(ID3D11Device* device, SDK_callbacks sdkCbs)
:m(device), sdkCallbacks(sdkCbs)
{
	minPower = .5f;
	maxPower = 3.0f;
	reset();
}

void Hiro::reset()
{
	r=0;
	captured=0;
	score=0;
	isHit = false;	
	tail = this;
	angle = 0;
	pos = XMFLOAT3(0,2,0);
	vel = XMFLOAT3(0,0,0);
	qPos = XMFLOAT4(0,0,0,1); 
	left = Pellet::getPellets().size();
}

Hiro::~Hiro(void)
{
}

void Hiro::getViewMatrix(XMFLOAT4X4* view)
{;

	// Build the view matrix.
	XMVECTOR xpos = XMVectorSet(pos.x,pos.y, pos.z,0);
	XMVECTOR position = XMVector3Normalize(xpos);
	position *= SphereWorld::instance()->getRadius() * 3;
	//position.y = 0;
	XMFLOAT3 cross;
		orientationToPos(getBehind(1), cross);
		XMVECTOR xcross = XMLoadFloat3(&cross);
		xcross = xpos - xcross;
		xcross = XMVector3Normalize(xcross);
		XMStoreFloat4x4(view, XMMatrixLookAtLH( position, xpos, xcross));

}

float fDot(const XMVECTOR& v1, const XMVECTOR& v2)
{
	return XMVectorGetX(XMVector3Dot(v1, v2));
}

bool Hiro::projectToMinimap(XMFLOAT3 worldPos, XMFLOAT2 &mapPos )
{
	XMVECTOR xpos = XMLoadFloat3(&pos);
	XMVECTOR normal(xpos);
		normal = XMVector3Normalize(normal);
	XMFLOAT3 cross;
		orientationToPos(getBehind(FOLLOW_OFFSET), cross);
	XMVECTOR xcross = XMLoadFloat3(&cross);
		xcross = xpos - xcross;
		xcross = XMVector3Normalize(xcross);
	//now on normal plane
		XMVECTOR xworldPos = XMLoadFloat3(&worldPos);
	xworldPos = xworldPos - (normal * (fDot(normal, xworldPos)));
	mapPos.x = fDot(xworldPos,xcross);
		xcross = XMVector3Cross( xcross, normal);
	mapPos.y = fDot(xworldPos, xcross);

	return  0 < fDot(xpos, xworldPos);
}
void Hiro::drawTail()
{
	int numPellets;
	wchar_t buffer[64];

	wstring map = Pellet::genPelletMap(numPellets, this);
			float ex = pos.x;
			float wy = pos.y;
			float t = (atan2f(wy, ex));
			float r = (sqrtf(wy * wy + ex * ex)/SphereWorld::getRadius());
			swprintf(buffer, 64 ,L"%.3f,%.3f", t,r);
			map.append( buffer );
	wsprintf(buffer, L"%d",numPellets);
	sdkCallbacks.updateMiniMap(buffer,(wchar_t*)map.c_str());
}
float randff()
{
	 return rand()/((float)RAND_MAX);
}
void Hiro::update(float dt)
{
	//Get input
	angle += UI::isKeyDown('A') * dt * 4.f - UI::isKeyDown('D') * dt * 4.f;
	//Calculate orbit:
	int total =  Pellet::getPellets().size();
	float curPower = minPower;
	//Scale power based on 
	curPower +=  (maxPower - minPower)* ((total - left + captured) / (float)total );
	move( dt * curPower );

	//collect coins
	for(unsigned int i=0; i < Pellet::getPellets().size(); i++)
	{
		Pellet* p = Pellet::getPellets()[i];
		if(p->isEnabled())
		{
			XMVECTOR dist = XMLoadFloat3(&pos) - XMLoadFloat3(&(p->getPos()));
			if( XMVectorGetX(XMVector3LengthSq(dist)) < 1.5 * 1.5)
			{
				if( !p->isCaught())
				{
					p->hide(tail);
					tail =p;
					captured++;
					sdkCallbacks.updateScore(captured, left);
				}
				else if(p->getLeader() != this)
				{
					isHit = true;
				}
			//drawTail();
		}
	}
	}

	//scoring:
	if(UI::isKeyClicked(UI::KEY_ENTER)||UI::isRemoteClicked(L"bank"))
	{
		tail->disable();
		score += (captured * (captured+1))/2;
		left -= captured;
        captured =0;
		sdkCallbacks.updateScore(captured, left);
	}
}

void Hiro::render(ID3D11DeviceContext* context)
{
	XMMATRIX mWorld = XMMatrixTranslation(pos.x,pos.y,pos.z);
		mWorld = XMLoadFloat4x4(&orientation)*mWorld;

	Camera::instance()->setWorld(mWorld);
	Camera::instance()->flushMatricies(context);
	m.render(context);
}
