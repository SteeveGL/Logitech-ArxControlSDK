#include "Model.h"

#include <vector>
using namespace std;
Model::Model(ID3D11Device* device, int subdivisions, float texColor)
{
	modelIB=0;
	modelVB=0;
	initializeSphere(device, subdivisions, true, texColor);
}

Model::~Model(void)
{
}

void Model::render(ID3D11DeviceContext* context)
{
	if(modelVB && modelIB)
	{
		UINT stride = sizeof(SimpleVertex);
		UINT offset = 0;
		context->IASetPrimitiveTopology(curTopology);
		context->IASetVertexBuffers(0,1,&modelVB, &stride, &offset);
		context->IASetIndexBuffer(modelIB, DXGI_FORMAT_R32_UINT, 0);

	//Last minute flush of buffer data.
	//	pass->Apply(0, context);
		context->DrawIndexed(indexCount,0,0);
	}
}

XMFLOAT3 normalized(XMFLOAT3 vert)
{
	float mag = sqrtf(vert.x * vert.x + vert.y * vert.y +vert.z * vert.z);
	return XMFLOAT3(vert.x/mag,vert.y/mag, vert.z/mag);
}
void genFace(int divisions, XMVECTOR offset, XMVECTOR iAxis, XMVECTOR jAxis, vector<SimpleVertex> &floorverts, bool normalize = true, float texColor = 0)
{
		
	float dt = 1.0f/(divisions - 1);
	for(int x = 0; x < divisions; x++)
	{
		for(int y = 0; y < divisions; y++)
		{
			SimpleVertex vert;

			float ex = (x * dt);
			float wy = (y * dt);
			
			XMStoreFloat3(& vert.pos, offset + (ex * iAxis) + (wy * jAxis));
			if(normalize) vert.pos = normalized(vert.pos);
			vert.normal = XMFLOAT3(vert.pos.x, vert.pos.y, vert.pos.z);
			vert.tex = XMFLOAT3(ex,wy, texColor);
			floorverts.push_back(vert);
		}

	}

}

bool Model::initializeSphere(ID3D11Device *device, int divisions, bool normalize, float texColor )
{
	
		curTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	vector<SimpleVertex> floorverts;

	XMVECTOR offset= XMVectorSet(-.5f, -.5f, -.5f,0);
	XMVECTOR iAxis = XMVectorSet(1,0,0,0);
	XMVECTOR jAxis = XMVectorSet(0,1,0,0);

	//create unit cube: front/back
		genFace(divisions,offset,iAxis, jAxis, floorverts,normalize, texColor );
	offset= XMVectorSet(-.5f, -.5f, .5f,0);
	iAxis = XMVectorSet(0,1,0,0);
	jAxis = XMVectorSet(1,0,0,0);
		genFace(divisions,offset,iAxis, jAxis, floorverts,normalize, texColor );

	//create top/bottom
	offset= XMVectorSet(-.5f, .5f, -.5f,0);
	iAxis = XMVectorSet(1,0,0,0);
	jAxis = XMVectorSet(0,0,1,0);
		genFace(divisions,offset,iAxis, jAxis, floorverts,normalize, texColor );
	offset= XMVectorSet(-.5f, -.5f, -.5f,0);
	iAxis = XMVectorSet(0,0,1,0);
	jAxis = XMVectorSet(1,0,0,0);
		genFace(divisions,offset,iAxis, jAxis, floorverts,normalize, texColor );
	
	//generate left/right
	offset= XMVectorSet(-.5f, -.5f, -.5f,0);
	iAxis = XMVectorSet(0,1,0,0);
	jAxis = XMVectorSet(0,0,1,0);
		genFace(divisions,offset,iAxis, jAxis, floorverts,normalize, texColor );
	offset= XMVectorSet(.5f, -.5f, -.5f,0);
	iAxis = XMVectorSet(0,0,1,0);
	jAxis = XMVectorSet(0,1,0,0);
		genFace(divisions,offset,iAxis, jAxis, floorverts,normalize, texColor );
	
	vector<UINT> findex;
	for(int i = 0; i < 6; i++)
	{
		for(int x = 0; x < divisions - 1; x++)
		{
			for(int y = 0; y < divisions - 1; y++)
			{
				UINT ul, ur, ll, lr;
				//row   + col + face
				ul = x + y * divisions  + i*divisions*divisions;
				ur = ul + 1;
				ll = ul + divisions;
				lr = ll + 1;

				findex.push_back(ul);
				findex.push_back(ur);
				findex.push_back(ll);
					
				findex.push_back(ur);
				findex.push_back(lr);
				findex.push_back(ll);
			}
		}
	}
	
		//Make the second one
	D3D11_BUFFER_DESC newvbd;
	newvbd.Usage = D3D11_USAGE_DEFAULT;
	newvbd.ByteWidth = sizeof(SimpleVertex) * floorverts.size();
	newvbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	newvbd.CPUAccessFlags = 0;
	newvbd.MiscFlags = 0;
	newvbd.StructureByteStride = 0;
	
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &floorverts[0];
	
	HR( device->CreateBuffer( &newvbd, &vinitData, &modelVB) );

		//index buffer time!
	indexCount = findex.size();

	D3D11_BUFFER_DESC ibuff;
	ibuff.Usage = D3D11_USAGE_IMMUTABLE;
	ibuff.ByteWidth = sizeof(UINT) * findex.size();
	ibuff.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibuff.CPUAccessFlags = 0;
	ibuff.MiscFlags = 0;
	ibuff.StructureByteStride = 0;
	
	//Create some vertex  buffers
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &findex[0];
	
	HR( device->CreateBuffer( &ibuff, &iinitData, &modelIB) );
	
	return true;
}