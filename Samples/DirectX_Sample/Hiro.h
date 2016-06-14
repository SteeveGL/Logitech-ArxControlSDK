#pragma once
#include "Model.h"
#include "SphereWalker.h"
#include "SDK_callbacks.h"

class Hiro : public SphereWalker
{
public:
	Hiro(ID3D11Device* device, SDK_callbacks sdkCbs);
	~Hiro(void);

	void getWorldMatrix(XMFLOAT4X4 *world);
	void getViewMatrix(XMFLOAT4X4 *view);
	void update(float dt);
	void render(ID3D11DeviceContext* context);
	int getCaptured() { return captured;};
	void drawTail();
	bool projectToMinimap(XMFLOAT3 worldPos, XMFLOAT2 &mapPos );
	bool isHit;
	void reset();
	int getScore(){return score;}
	void disable(){ tail = this;};
	int captured;
	int score;
	int left;
private:
	SphereWalker* tail;
	Model m;
	XMFLOAT3 vel;
	float r;
	float minPower;
	float maxPower;
	SDK_callbacks sdkCallbacks;
};
