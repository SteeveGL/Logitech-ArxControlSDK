#include "Pellet.h"
#include "SphereWorld.h"
#include "Camera.h"
#include <string>
using namespace std;
vector<Pellet*> Pellet::list;
Model* Pellet::m = NULL;
float randf()
{
	 return rand()/((float)RAND_MAX);
}
Pellet::Pellet(ID3D11Device* device)
{
	reset();
	if(m == NULL)
	{
		m = new Model(device,2);
	}
	list.push_back(this);

}

void Pellet::resetAll()
{
	for(unsigned int i=0; i < list.size(); i++)
	{
		list[i]->reset();
	}
}

void Pellet::reset()
{
	leader =0;
	caught = false;
	enabled = true;
	angle = rand()%(360*4) / 180.0f;
	XMVECTOR randAxis =  XMVectorSet(randf()* 10 - 5,randf()* 10 - 5,randf()* 10 - 5,0);
	XMStoreFloat4(&qPos, XMQuaternionRotationAxis(randAxis, randf() *360 - 180));
	pos = XMFLOAT3(0,0,0);
}

Pellet::~Pellet(void)
{
	for(unsigned int i =0; i < list.size(); i++)
	{
		if(list[i] ==this)
		{
			list.erase(list.begin()+i);
			break;
		}
	}
	if(list.size() ==0)
	{
		delete m;
	}
}

void Pellet::render(ID3D11DeviceContext *context)
{	
	XMMATRIX xScale;
	xScale = XMMatrixScaling(.5f,caught?1:.5f,.5f);
	XMMATRIX mWorld; 
	XMVECTOR xPos = XMLoadFloat3(&pos);
		xPos *= offset;
	mWorld = XMMatrixTranslationFromVector(xPos);

	XMMATRIX xorientation = XMLoadFloat4x4(&orientation);
	mWorld = xScale*xorientation*mWorld;

	Camera::instance()->setWorld(mWorld);
	Camera::instance()->flushMatricies(context);
	m->render(context);
}

void Pellet::renderAll(ID3D11DeviceContext *context)
{
	for(unsigned int i =0; i < list.size(); i++)
	{
		if(list[i]->enabled)
		{
			list[i]->render(context);
		}
	}
}

void Pellet::update(float dt)
{
	//list[i]->angle += dt * ((rand()/((float)RAND_MAX))  * 5 - 2.5);

	if(isCaught() && leader != NULL)
	{
		XMVECTOR desiredPos = XMLoadFloat4(&(leader->getBehind(FOLLOW_OFFSET)));
		angle = (dt*7) * leader->getAngle() + (1-dt*7) * angle;
		XMStoreFloat4(&qPos, XMQuaternionSlerp(XMLoadFloat4(&qPos),desiredPos, dt*20));
	}
	move(0);
	
}

void Pellet::updateAll(float dt)
{
	static float t = 0;
	t+= dt*4;
	for(unsigned int i =0; i < list.size(); i++)
	{
		if(list[i]->enabled)
		{
			list[i]->offset = 1 + abs(sin(t + i) * 0.04f);
			list[i]->update(dt);
		}
	}
}
wstring Pellet::genPelletMap(int &numPellets, Hiro* h)
{
	wstring map;
	numPellets =0;
	WCHAR buffer[100];
	for(unsigned int i=0; i < list.size(); i++)
	{
		Pellet* p = list[i];

			XMFLOAT2 mapPos;
			h->projectToMinimap(p->pos, mapPos);
		if(p->isCaught())
		{
			numPellets++;
			float ex = p->pos.x;
			float wy = p->pos.y;
			float t = (atan2f(wy, ex));
			float r = (sqrtf(wy * wy + ex * ex)/SphereWorld::getRadius());
			swprintf(buffer, 99 ,L"%.3f,%.3f,", t,r);
			map.append( buffer );
		}
	}
//	wprintf(L"%d >%s<", map.length(),  (wchar_t *)map.c_str());
	return map;
}

void Pellet::disable()
{
	enabled = false;
	caught = false;
	if(leader != NULL)
	{
		leader->disable();
	}
}