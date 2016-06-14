#pragma once
#include <windows.h>
#pragma message("TODO: Upgrade to win8sdk version of d3d11.h; change compiler back to (v110)")
#include <d3dx11.h>
typedef LRESULT (CALLBACK*WindowCallback)( HWND, UINT, WPARAM, LPARAM );

class Renderer
{
public:
	static Renderer* instance();
	int init(HINSTANCE hInstance, int nCmdShow, WindowCallback Wndproc);
	ID3D11DeviceContext* context(){return g_pImmediateContext;};
	ID3D11Device* device(){ return g_pd3dDevice;};
	HWND hwnd(){
		return g_hWnd;
	}
	void clearDepth()
	{
		context()->ClearDepthStencilView( g_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0 );
	}
	void clearView(float clearColor[4])
	{
		clearDepth();
		context()->ClearRenderTargetView( g_pRenderTargetView, clearColor );
	}
	void present()
	{
		g_pSwapChain->Present( 0, 0 );
	}
	void CleanupDevice();

	void useLogo()
	{
		context()->PSSetShaderResources( 0, 1, &gLogo );
	}
	void useGUI()
	{	
		context()->PSSetShaderResources( 0, 1, &spritesheet );
	}
private:
	Renderer(void);
	~Renderer(void);
	HRESULT InitWindow( HINSTANCE hInstance, int nCmdShow,  WindowCallback Wndproc);
	HRESULT InitDevice();

	static Renderer* ins;

	HINSTANCE               g_hInst ;
	HWND                    g_hWnd ;
	D3D_DRIVER_TYPE         g_driverType ;
	D3D_FEATURE_LEVEL       g_featureLevel ;
	ID3D11Device*           g_pd3dDevice ;
	ID3D11DeviceContext*    g_pImmediateContext ;
	IDXGISwapChain*         g_pSwapChain ;
	ID3D11RenderTargetView* g_pRenderTargetView ;
	ID3D11Texture2D*        g_pDepthStencil ;
	ID3D11DepthStencilView* g_pDepthStencilView ;
	ID3D11VertexShader*     g_pVertexShader ;
	ID3D11PixelShader*      g_pPixelShader ;
	ID3D11InputLayout*      g_pVertexLayout ;
//texture
ID3D11ShaderResourceView* spritesheet;
ID3D11ShaderResourceView* gLogo;
ID3D11SamplerState* g_pSamplerLinear;
};
