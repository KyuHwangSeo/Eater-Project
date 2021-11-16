#include "MainHeader.hlsli"

struct VertexInputType
{
	float3 PosL			: POSITION;
	float2 Tex			: TEXCOORD;
    float3 Nomal		: NORMAL;
    float3 Tangent		: TANGENT;
};

struct PixelInputType
{
    float4 posH     : SV_POSITION;
    float2 Tex      : TEXCOORD;
    float3 Nomal    : NORMAL;
    float3 Tangent  : TANGENT;
};


PixelInputType main(VertexInputType input)
{
    PixelInputType output;
   
	//월드변환
	
    output.posH = mul(float4(input.PosL.xyz, 1.0f), world);
    output.posH = mul(float4(output.posH.xyz, 1.0f), view);
    output.posH = mul(float4(output.posH.xyz, 1.0f), proj);
    
    output.Tex = float2(0, 0);
    output.Nomal = mul(input.Nomal, (float3x3) view);
    output.Nomal = mul(output.Nomal, (float3x3) proj);
    
    output.Tangent = mul(input.Tangent, (float3x3) world);
    
    
    
   
    return output;
}