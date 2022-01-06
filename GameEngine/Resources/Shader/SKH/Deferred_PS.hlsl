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
    float4 MaskColor    : MASK_COLOR;
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

float2 Voronoi_RandomVector(float2 UV, float offset)
{
    float2x2 m = float2x2(15.27, 47.63, 99.41, 89.98);
    UV = frac(sin(mul(UV, m)) * 46839.32);
    return float2(sin(UV.y * +offset) * 0.5 + 0.5, cos(UV.x * offset) * 0.5 + 0.5);
}

PixelOut Deferred_PS(VertexIn pin)
{
    PixelOut pout;

    float4 albedo = gColor;

    float3 normalW = pin.NormalW;
    float3 normalV = pin.NormalV;
    
    //float2 g = floor(pin.Tex);
    //float2 f = frac(pin.Tex);
    //float t = 8.0f;
    //float3 res = float3(8.0f, 0.0f, 0.0f);
    //float2 UV = pin.Tex;
    //for (int y = -1; y <= 1; y++)
    //{
    //    for (int x = -1; x <= 1; x++)
    //    {
    //        float2 lattice = float2(x, y);
    //        float2 offset = Voronoi_RandomVector(lattice + g, 1);
    //        float d = distance(lattice + offset, f);
    //        if (d < res.x)
    //        {
    //            res = float3(d, offset.x, offset.y);
    //            UV.x = res.x;
    //            UV.y = res.y;
    //        }
    //    }
    //}
    
#ifdef TERRAIN_MESH
    albedo = float4(0.0f, 0.0f, 0.0f, 1.0f);
    
    float4 mask = float4(pin.MaskColor.rgb, 1.0f);
    mask.rg /= mask.r + mask.g;
    
    float3 normalColor = float3(0.0f, 0.0f, 0.0f);

    if (mask.r > 0.0f)
    {
        albedo.rgb += gDiffuseLayer1.Sample(gSamWrapLinear, pin.Tex).rgb * mask.r;
        normalColor += gNormalLayer1.Sample(gSamWrapLinear, pin.Tex).rgb* mask.r;
    }
    if (mask.g > 0.0f)
    {
        albedo.rgb += gDiffuseLayer2.Sample(gSamWrapLinear, pin.Tex).rgb * mask.g;
        normalColor += gNormalLayer2.Sample(gSamWrapLinear, pin.Tex).rgb* mask.g;
    }
    
    float3 normalSample = (2.0f * normalColor - 1.0f);
    
    normalW = mul(normalSample, pin.TBNW);
    normalV = mul(normalSample, pin.TBNV);
#else
    if (gTexID & ALBEDO_MAP)
    {
        albedo = gDiffuseMap.Sample(gSamWrapLinear, pin.Tex);
    }
    
    if (gTexID & NORMAL_MAP)
    {
        normalW = mul(2.0f * gNormalMap.Sample(gSamWrapLinear, pin.Tex).rgb - 1.0f, pin.TBNW);
        normalV = mul(2.0f * gNormalMap.Sample(gSamWrapLinear, pin.Tex).rgb - 1.0f, pin.TBNV);
    }
#endif
    
    // Gamma Correction
	// Gamma Space -> Linear Space
	// 모든 라이팅 연산은 선형 공간에서 이루어져야 한다..
#ifdef GAMMA_CORRECTION
    albedo.rgb = pow(albedo.rgb, 2.2f);
#endif
    
    pout.Albedo = albedo;
    pout.Normal = float4(normalW, 0.0f);
    pout.Position = float4(pin.PosW, gMatID);
    pout.Shadow = float4(pin.ShadowPosH.xyz, 0.0f);
    pout.Depth = float4(normalV, pin.PosV.z);
    
    return pout;
}