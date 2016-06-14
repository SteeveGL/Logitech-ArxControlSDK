//--------------------------------------------------------------------------------------
// File: PixelShader.fx
//
//--------------------------------------------------------------------------------------

#include "ShaderDefs.fx"

cbuffer cbPerFrame
{
	float4 colorOverlay;
}

Texture2D gDiffuseMap;

SamplerState samUI
{
	Filter = MIN_MAG_MIP_POINT;

	AddressU = WRAP;
	AddressV = WRAP;
};

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( VertexOut vin ) : SV_Target
{
	float4 texColor = float4(1,0,1,1);

	texColor = gDiffuseMap.Sample( samUI, vin.Tex.xy );
	float4 normalColor = float4(vin.NormalW, 1.0f ) + float4(.5,.5,.5,0); 
	
    texColor = lerp(normalColor, texColor, vin.Tex.z); //   // Yellow, with Alpha = 1
    return float4(lerp(texColor.xyz, colorOverlay.xyz, colorOverlay.w), texColor.z);
}
