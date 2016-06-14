#pragma once

#include <d3dx11.h>
#include <d3dcompiler.h>
#include <xnamath.h>
#include "resource1.h"

struct SimpleVertex{
	XMFLOAT3 pos;
	XMFLOAT3 normal;
	XMFLOAT3 tex;
};

class Model
{
public:
	Model(ID3D11Device *device, int subdivisions =3, float texColor = 0);
	~Model(void);
	void  render(ID3D11DeviceContext* device);
private:
	bool initializeSphere(ID3D11Device *device, int divisions, bool normalize, float texColor=0);
	ID3D11Buffer* modelVB;
	ID3D11Buffer* modelIB;
	int indexCount;
	D3D11_PRIMITIVE_TOPOLOGY curTopology;
	const static char* geometryGenerators[];
};
