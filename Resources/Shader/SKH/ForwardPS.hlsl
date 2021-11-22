#include "LightHelper.fx"

cbuffer cbLights : register(b0)
{
    DirectionalLight gDirLights[3];
    PointLight gPointLights[5];
    SpotLight gSpotLights[5];

    uint gPointLightCount;
    uint gSpotLightCount;
};

cbuffer cbCamera : register(b1)
{
    float3 gEyePosW : packoffset(c0);
};

cbuffer cbMaterials : register(b2)
{
    Material gMaterials[5];
}

Texture2D gDiffuseMap : register(t0);
Texture2D gNormalMap : register(t1);
Texture2D gShadowMap : register(t2);

SamplerComparisonState gShadowSam : register(s0);
SamplerState samWrapMinLinear : register(s1);

struct PixelIn
{
    float4 PosW : SV_POSITION;
    float2 Tex : TEXCOORD;
    float3 NormalW : NORMALW;
    float3 ShadowPosH : POS_SHADOW;
    
    float3x3 TBN : TANGENT;
};

float4 main(PixelIn pin) : SV_Target0
{
    float4 albedo = gDiffuseMap.Sample(samWrapMinLinear, pin.Tex);
    
    // Gamma Correction
	// Gamma Space -> Linear Space
	// 모든 라이팅 연산은 선형 공간에서 이루어져야 한다..
    albedo.rgb = pow(albedo.rgb, 2.2);
    
    float3 normalMapSample = 2.0f * gNormalMap.Sample(samWrapMinLinear, pin.Tex).rgb - 1.0f;
    float3 bumpedNormalW = mul(normalMapSample, pin.TBN);
    
    // Shadow
    float shadows = CalcShadowFactor(gShadowSam, gShadowMap, pin.ShadowPosH);
    
    // View Direction
    float3 ViewDirection = pin.PosW.xyz - gEyePosW;
    ViewDirection = normalize(ViewDirection);
    
    // Start with a sum of zero.
    float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 spec = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    float4 A, D, S;
    
	float4 litColor = albedo;
    
    // Directional Light
	[unroll]
    for (uint i = 0; i < 1; ++i)
    {
        ComputeDirectionalLight(gMaterials[0], gDirLights[i], bumpedNormalW, ViewDirection,
				A, D, S);
    
        ambient += A;
        diffuse += shadows * D;
        spec += shadows * S;
    }
    
    // Point Light
    if (gPointLightCount > 0)
    {
			[unroll]
        for (uint i = 0; i < gPointLightCount; ++i)
        {
            ComputePointLight(gMaterials[0], gPointLights[i], pin.PosW.xyz, bumpedNormalW, ViewDirection,
					A, D, S);
    
            ambient += A;
            diffuse += D;
            spec += S;
        }
    }
    
    // Spot Light
    if (gSpotLightCount > 0)
    {
			[unroll]
        for (uint i = 0; i < gSpotLightCount; ++i)
        {
            ComputeSpotLight(gMaterials[0], gSpotLights[i], pin.PosW.xyz, bumpedNormalW, ViewDirection,
					A, D, S);
    
            ambient += A;
            diffuse += D;
            spec += S;
        }
    }
    
    // Modulate with late add.
    litColor = albedo * (ambient + diffuse) + spec;
    
    // Common to take alpha from diffuse material and texture.
    litColor.a = gMaterials[0].Diffuse.a * albedo.a;
    
    
    return litColor;
   // return float4(1.0f, 0.0f, 0.0f, 1.0f);
}