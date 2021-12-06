#define ALBEDO_MAP 0x00000001
#define NORMAL_MAP 0x00000010

cbuffer cbMaterial : register(b0)
{
    float4 gColor   : packoffset(c0);
    uint gMatID     : packoffset(c1.x);
    uint gTexID     : packoffset(c1.y);
};

Texture2D gDiffuseMap : register(t0);
Texture2D gNormalMap : register(t1);

SamplerState gSamWrapLinear : register(s0);

struct VertexIn
{
    float4 PosH : SV_POSITION;
    float3 PosW : POSITIONW;
    float3 PosV : POSITIONV;
    float3 NormalW : NORMALW;
    float3 NormalV : NORMALV;
    float2 Tex : TEXCOORD;
    float3 ShadowPosH : POS_SHADOW;
    float3x3 TBN : TANGENT;
};

struct PixelOut
{
    float4 Albedo       : SV_Target0;
    float4 Normal       : SV_Target1;
    float4 Position     : SV_Target2;
    float4 Shadow       : SV_Target3;
    float4 NormalDepth  : SV_Target4;
};

PixelOut main(VertexIn pin)
{
	PixelOut vout;

    float4 albedo = gColor;
    float3 normal = pin.NormalW;
    
    if (gTexID & ALBEDO_MAP)
    {
        albedo = gDiffuseMap.Sample(gSamWrapLinear, pin.Tex);
    }
    
    if (gTexID & NORMAL_MAP)
    {
        float3 normalMapSample = 2.0f * gNormalMap.Sample(gSamWrapLinear, pin.Tex).rgb - 1.0f;
        normal = mul(normalMapSample, pin.TBN);
    }
	
    vout.Albedo = albedo;
    vout.Normal = float4(normal, 1.0f);
    vout.Position = float4(pin.PosW, gMatID);
    vout.Shadow = float4(pin.ShadowPosH.xyz, 0.0f);
    vout.NormalDepth = float4(pin.NormalV.xyz, pin.PosV.z);
    
	return vout;
}