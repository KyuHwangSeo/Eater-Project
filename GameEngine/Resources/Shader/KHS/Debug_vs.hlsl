#include "MainHeader.hlsli"

struct VertexInputType
{
	float3 PosL			: POSITION;
	float3 Color		: COLOR;
};

struct PixelInputType
{
    float4 posH     : SV_POSITION;
    float3 Tex      : COLOR;
};


PixelInputType main(VertexInputType input)
{
    PixelInputType output;
   
	//월드변환
	
    output.posH = mul(float4(input.PosL.xyz, 1.0f), world);
    output.posH = mul(float4(output.posH.xyz, 1.0f), view);
    output.posH = mul(float4(output.posH.xyz, 1.0f), proj);
    
    output.Tex = input.Color;

    return output;
}