//--------------------------------------------------------------------------------------
// File: SimpleShader.fx
//
//--------------------------------------------------------------------------------------

struct VertexIn
{
	float3 PosL    : POSITION;
    float3 NormalL : NORMAL;
	float3 Tex     : TEXCOORD;
};
struct VertexOut
{
	float4 PosH  : SV_POSITION;
	float3 PosW	 : POSITION;
    float3 NormalW : NORMAL;
	float3 Tex : TEXCOORD;
};