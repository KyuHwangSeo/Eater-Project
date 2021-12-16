#define ALBEDO_MAP 0x00000001
#define NORMAL_MAP 0x00000010

cbuffer cbMaterial : register(b0)
{
    float4 gColor   : packoffset(c0);
    uint gMatID     : packoffset(c1.x);
    uint gTexID     : packoffset(c1.y);
};

#ifdef TERRAIN_MESH
Texture2D gDiffuseLayer1    : register(t0);
Texture2D gNormalLayer1     : register(t1);
Texture2D gDiffuseLayer2    : register(t2);
Texture2D gNormalLayer2     : register(t3);
#else
Texture2D gDiffuseMap : register(t0);
Texture2D gNormalMap : register(t1);
#endif

SamplerState gSamWrapLinear : register(s0);

struct VertexIn
{
    float4 PosH         : SV_POSITION;
    float3 PosW         : POSITIONW;
    float3 PosV         : POSITIONV;
    float3 NormalW      : NORMALW;
    float3 NormalV      : NORMALV;
    float2 Tex          : TEXCOORD;
    float3 ShadowPosH   : POS_SHADOW;
    
    float3x3 TBNW       : TANGENTW;
    float3x3 TBNV       : TANGENTV;
    
#ifdef TERRAIN_MESH
    float4 MaskColor : MASK;
#endif
};


struct PixelOut
{
    float4 Albedo   : SV_Target0;
    float4 Normal   : SV_Target1;
    float4 Position : SV_Target2;
    float4 Shadow   : SV_Target3;
    float4 Depth    : SV_Target4;
};

PixelOut Deferred_PS(VertexIn pin)
{
    PixelOut vout;

    float4 albedo = gColor;

    float3 normalW = pin.NormalW;
    float3 normalV = pin.NormalV;
    float gamma = 0.0f;
    
#ifdef TERRAIN_MESH
    albedo = float4(0.0f, 0.0f, 0.0f, 1.0f);
    
    float4 mask = float4(pin.MaskColor.rgb, 1.0f);
    mask.rg /= mask.r + mask.g;
    
    float3 normalColor = float3(0.0f, 0.0f, 0.0f);
    float3 normalSample = float3(0.0f, 0.0f, 0.0f);
    if (mask.r > 0.0f)
    {
        albedo.rgb += gDiffuseLayer1.Sample(gSamWrapLinear, pin.Tex).rgb * mask.r;
        normalColor = gNormalLayer1.Sample(gSamWrapLinear, pin.Tex).rgb;
        normalSample += (2.0f * normalColor - 1.0f) * mask.r;
    }
    if (mask.g > 0.0f)
    {
        albedo.rgb += gDiffuseLayer2.Sample(gSamWrapLinear, pin.Tex).rgb * mask.g;
        normalColor = gNormalLayer2.Sample(gSamWrapLinear, pin.Tex).rgb;
        normalSample += (2.0f * normalColor - 1.0f) * mask.g;
    }
    
    normalW = mul(normalSample, pin.TBNW);
    normalV = mul(normalSample, pin.TBNV);
#else
    if (gTexID & ALBEDO_MAP)
    {
        albedo = gDiffuseMap.Sample(gSamWrapLinear, pin.Tex);
        gamma = 0.0f;
    }
    
    if (gTexID & NORMAL_MAP)
    {
        normalW = mul(2.0f * gNormalMap.Sample(gSamWrapLinear, pin.Tex).rgb - 1.0f, pin.TBNW);
        normalV = mul(2.0f * gNormalMap.Sample(gSamWrapLinear, pin.Tex).rgb - 1.0f, pin.TBNV);
        gamma = 1.0f;
    }
#endif
    
    vout.Albedo = albedo;
    vout.Normal = float4(normalW, gamma);
    vout.Position = float4(pin.PosW, gMatID);
    vout.Shadow = float4(pin.ShadowPosH.xyz, 0.0f);
    vout.Depth = float4(normalV, pin.PosV.z);
    
    return vout;
}