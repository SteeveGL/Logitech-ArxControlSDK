#pragma once
#include "Model.h"
class SphereWalker
{
public:
	
	XMFLOAT3 getPos() { return pos;};
	XMFLOAT4 getSurfacePos() { return qPos;};
	float getAngle(){return angle;};
	XMFLOAT4 getBehind(float dist);
	void orientationToPos(XMFLOAT4 orientation, XMFLOAT3 &pos);
	virtual void disable() =0;
	bool isEnabled(){return enabled;};
 
protected:
	bool enabled;
	void getWorldMatrix(XMFLOAT4X4 *world);	
	void move(float dt);
	float angle;
	XMFLOAT4 qPos;
	XMFLOAT3 pos;
	XMFLOAT4X4 orientation;
};
