#include "Camera.h"

Camera* Camera::main = NULL;

void Camera::init(ID3D11Device* dev)
{
	main = new Camera(dev);
}
Camera::Camera(ID3D11Device* dev)
:objectCBuffer(0),
g_iCBVSPerObjectBind(0), 
g_iCBPSPerObjectBind(0)
{

  // Setup constant buffers
	D3D11_BUFFER_DESC Desc;
    Desc.Usage = D3D11_USAGE_DYNAMIC;
    Desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    Desc.MiscFlags = 0;
    Desc.ByteWidth = sizeof( CB_VS_PER_OBJECT );
     dev->CreateBuffer( &Desc, NULL, &objectCBuffer );

    Desc.Usage = D3D11_USAGE_DYNAMIC;
    Desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    Desc.MiscFlags = 0;
    Desc.ByteWidth = sizeof( CB_PS_PER_FRAME );
	dev->CreateBuffer( &Desc, NULL, &pixelCBuffer);

	 mTexTransform=XMFLOAT4(0,0,1,1);
	 mColorOverlay = XMFLOAT4(1,0,0,.4f);
}

Camera::~Camera(void)
{


}

void Camera::flushMatricies(ID3D11DeviceContext* context)
{
	
	XMMATRIX xWorldViewProjection = XMLoadFloat4x4(&mWorld) *XMLoadFloat4x4(&mView)* XMLoadFloat4x4(&mProj);

    // Per frame cb update
    D3D11_MAPPED_SUBRESOURCE MappedResource;
        
    // VS Per object
    context->Map( objectCBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource ) ;

    CB_VS_PER_OBJECT* pVSPerObject = ( CB_VS_PER_OBJECT* )MappedResource.pData;
    XMStoreFloat4x4( &pVSPerObject->m_WorldViewProj, XMMatrixTranspose( xWorldViewProjection ));
    XMStoreFloat4x4( &pVSPerObject->m_World, XMMatrixTranspose(xWorldViewProjection ));
	pVSPerObject->m_TexTransform = mTexTransform;
    context->Unmap( objectCBuffer, 0 );
    context->VSSetConstantBuffers( g_iCBVSPerObjectBind, 1, &objectCBuffer );

	context->Map(pixelCBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
	CB_PS_PER_FRAME* pPsPerFrame = (CB_PS_PER_FRAME*)MappedResource.pData;
	pPsPerFrame->m_overlay =  mColorOverlay ;
	context->Unmap(pixelCBuffer, 0 );
	context->PSSetConstantBuffers( g_iCBPSPerObjectBind, 1, &pixelCBuffer );
}