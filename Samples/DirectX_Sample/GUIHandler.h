#pragma once
#include <d3dx11.h>
#include <vector>
using namespace std;
#include "resource1.h"
#include "SDK_callbacks.h"
#include "Model.h"


#define NUMVERTS 256
#define TEXSTEP (.25f)

struct Letter
{
	XMFLOAT4X4 pos;
	XMFLOAT4 glyph;
};


class GUIHandler
{
public:
	GUIHandler(ID3D11Device*  device);
	~GUIHandler(void);
	void render(ID3D11DeviceContext*  context);

	void printScore(int cur, int max);
	void printStatus(CONNECT_STATUS status, char orientation);
	void printFPS(float dt);
private:
	vector<Letter> letters;
	Letter statusIcon;
	Model* facet;
	float fps;
};
