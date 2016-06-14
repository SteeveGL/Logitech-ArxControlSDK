#include "SphereWalker.h"
#include "SphereWorld.h"

XMFLOAT4 SphereWalker::getBehind(float dist)
{
	XMVECTOR xperp	  = XMVectorSet(-cosf(angle), sinf(angle), 0,0);
	XMVECTOR incrementalRotation;
	incrementalRotation = XMQuaternionRotationAxis( xperp, -dist);
	XMFLOAT4 ret;
	XMStoreFloat4(&ret, XMQuaternionMultiply(incrementalRotation , XMLoadFloat4(&qPos)));
	return ret;
}
void SphereWalker::orientationToPos(XMFLOAT4 qorientation, XMFLOAT3 &npos)
{
	float r = SphereWorld::instance()->getRadius();
	XMVECTOR xorientation = XMLoadFloat4(&qorientation);
	xorientation = XMQuaternionNormalize(xorientation);
	XMVECTOR qvec= XMVectorSet(0,0,r,0);
	qvec = XMVector3Rotate(qvec, xorientation);
	XMStoreFloat3(&npos, qvec);
}

void SphereWalker::move(float dt)
{
	XMVECTOR	perp	  = XMVectorSet(-cosf(angle), sinf(angle), 0,0);
	XMVECTOR incrementalRotation;
	incrementalRotation = XMQuaternionRotationAxis(perp, dt );
	XMStoreFloat4(&qPos, XMQuaternionMultiply(incrementalRotation , XMLoadFloat4(&qPos)));
	
	orientationToPos(qPos, pos);
	
	//Rotate in space
	XMMATRIX globalRotation = XMMatrixRotationQuaternion( XMLoadFloat4(&qPos));
	
	XMMATRIX localRotation;
		XMVECTOR up = XMVectorSet( 0,0,1,0);
		localRotation = XMMatrixRotationAxis(up, -angle);
	XMStoreFloat4x4(&orientation, localRotation * globalRotation);

}
