#pragma once
#include <vector>
using namespace std;
#include "Model.h"
#include "SphereWalker.h"
#include "Hiro.h"
#define FOLLOW_OFFSET .05f

class Pellet : public SphereWalker
{
public:
	Pellet(ID3D11Device* device);
	~Pellet(void);
	static void updateAll(float dt);
	void update(float dt);
	static void renderAll(ID3D11DeviceContext* context);
	void render(ID3D11DeviceContext* context);
	static vector<Pellet*> &getPellets(){return list;};
	void hide(SphereWalker* tail) { 
		leader = tail;
		qPos = leader->getBehind(FOLLOW_OFFSET);
		angle = leader->getAngle();
		caught = true;
	};
	SphereWalker* getLeader(){return leader;};
	bool isCaught(){return caught;};
	static wstring genPelletMap(int &numPellets, Hiro* h);
	static void resetAll();
	void reset();
	void disable();
private:
	static vector<Pellet*> list;
	static Model* m;
	SphereWalker* leader;
	float offset;
	bool caught;
};
