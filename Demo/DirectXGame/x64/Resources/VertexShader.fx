//--------------------------------------------------------------------------------------
// File: VertexShader.fx
//
//--------------------------------------------------------------------------------------

#include "ShaderDefs.fx"
//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
cbuffer cbPerObject
{
	float4x4 gWorldViewProj;
	float4x4 gWorld;
	//float4x4 gWorldInvTranspose;
	float4  gTexTransform;
};



VertexOut VS( VertexIn vin)
{
VertexOut vout;
    vout.PosH =    mul( float4(vin.PosL,1.0f) , gWorldViewProj  ) ;
    vout.PosW = vin.PosL;
    vout.NormalW = vin.NormalL;
    vout.Tex.z = vin.Tex.z;
    vout.Tex.xy = (vin.Tex.xy + gTexTransform.xy) * gTexTransform.zw;
    
    return vout;
}