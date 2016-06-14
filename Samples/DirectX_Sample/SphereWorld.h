#pragma once
#include "Model.h"

class SphereWorld
{
public:
	static void init(ID3D11Device* device);
	static SphereWorld* instance();

	void render(ID3D11DeviceContext* context);
	XMFLOAT3 getWorldPos(XMFLOAT3 sphereCoord);
	void getWorldMatrix(XMFLOAT3 coord, XMFLOAT4X4 *world);
	static float getRadius(){return main->radius;};

private:
	static SphereWorld * main;
	SphereWorld(ID3D11Device* device);
	~SphereWorld(void);

	Model m;
	float radius;
};
