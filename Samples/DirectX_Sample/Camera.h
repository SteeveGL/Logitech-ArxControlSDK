#pragma once

#include <d3dx11.h>
#include "Model.h"

struct CB_VS_PER_OBJECT
{
    XMFLOAT4X4 m_WorldViewProj;
    XMFLOAT4X4 m_World;
	XMFLOAT4 m_TexTransform;
};
struct CB_PS_PER_FRAME
{
	XMFLOAT4 m_overlay;
};
class Camera
{
public:
	static void init(ID3D11Device* dev);
	static Camera* instance(){ return main;}
	void flushMatricies(ID3D11DeviceContext* context);
	void setWorld(XMFLOAT4X4* world){mWorld =XMFLOAT4X4(*world);};
	void setView(XMFLOAT4X4* view)	{mView =XMFLOAT4X4(*view);};
	void setProj(XMFLOAT4X4* proj)	{mProj =XMFLOAT4X4(*proj);};
	void setTexTransform(XMFLOAT4 trans){ mTexTransform=XMFLOAT4(trans);};
	void setColor(XMFLOAT4 color) {mColorOverlay=XMFLOAT4(color);};
	void setWorld(XMMATRIX& world)	{XMStoreFloat4x4(&mWorld, world);};
	void setView (XMMATRIX& view)	{XMStoreFloat4x4(&mView, view);};
	void setProj (XMMATRIX& proj)	{XMStoreFloat4x4(&mProj, proj);};
	void setTexTransform(XMVECTOR &trans){XMStoreFloat4(&mTexTransform,trans);};
	void setColor(XMVECTOR &color)		 {XMStoreFloat4(&mColorOverlay, color);};

private:
	Camera(ID3D11Device* dev);
	~Camera(void);
	static Camera* main;

ID3D11Buffer* objectCBuffer;
ID3D11Buffer* pixelCBuffer;

UINT g_iCBVSPerObjectBind;
UINT g_iCBPSPerObjectBind;
	XMFLOAT4X4 mWorld;
    XMFLOAT4X4 mView;
    XMFLOAT4X4 mProj;
	XMFLOAT4 mTexTransform;
	XMFLOAT4 mColorOverlay;
};
