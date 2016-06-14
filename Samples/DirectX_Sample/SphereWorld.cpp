#include "SphereWorld.h"
#include "Camera.h"

SphereWorld* SphereWorld::main = NULL;

void SphereWorld::init(ID3D11Device* device)
{
	main = new SphereWorld(device);
}

SphereWorld* SphereWorld::instance(){
	return main;
}

SphereWorld::SphereWorld(ID3D11Device* device)
:m(device, 20, 1)
{
	radius = 25;
}

SphereWorld::~SphereWorld(void)
{
}

void SphereWorld::getWorldMatrix(XMFLOAT3 coord, XMFLOAT4X4 *world)
{
	XMFLOAT3 spherePos = getWorldPos(coord);
	XMStoreFloat4x4(world, XMMatrixTranslationFromVector( XMLoadFloat3( &spherePos ) ));
}

XMFLOAT3 SphereWorld::getWorldPos(XMFLOAT3 coord)
{
	float x, y, z;
	x = cosf(coord.y)*sinf(coord.x)*radius*coord.z;
	y = sinf(coord.y)*sinf(coord.x)*radius*coord.z;
	z = cosf(coord.x)			   *radius*coord.z;

	return XMFLOAT3(x,y,z);

}

void SphereWorld::render(ID3D11DeviceContext *context)
{
	static float t = 0; 
	t+= .001f;
	XMFLOAT4X4 mWorld;
	XMStoreFloat4x4(&mWorld, XMMatrixScaling(radius,radius,radius));
	Camera::instance()->setWorld(&mWorld);	
	Camera::instance()->flushMatricies(context);
	m.render(context);
}