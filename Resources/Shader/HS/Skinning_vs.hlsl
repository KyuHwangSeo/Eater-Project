#include "MainHeader.hlsli"

struct VertexInputType
{
    float3 PosL : POSITION;
    float2 Tex : TEXCOORD;
    float3 Nomal : NORMAL;
    float3 Tangent : TANGENT;
    uint4 BoneIndex0 : BONEINDEX;
    float4 BoneWeights0 : BONEWEIGHT;
};

struct PixelInputType
{
    float4 posH : SV_POSITION;
    float2 Tex : TEXCOORD;
    float3 Nomal : NORMAL;
    float3 Tangent : TANGENT;
};


PixelInputType main(VertexInputType input)
{
    PixelInputType output;
   
	//월드변환
    float weights[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    weights[0] = input.BoneWeights0.x;
    weights[1] = input.BoneWeights0.y;
    weights[2] = input.BoneWeights0.z;
    weights[3] = 1.0f - weights[0] - weights[1] - weights[2];
    
    float3 posL = float3(0.0f, 0.0f, 0.0f);
    float3 normalL = float3(0.0f, 0.0f, 0.0f);
    
    for (int i = 0; i < 4; i++)
    {
        posL += weights[i] * mul(float4(input.PosL, 1.0f), BoneOffset[input.BoneIndex0[i]]).xyz;
        normalL += weights[i] * mul(input.Nomal, (float3x3) BoneOffset[input.BoneIndex0[i]]);
    }
    
    
    output.posH = mul(float4(posL, 1.0f), Skinning_world).xyzw;
    output.posH = mul(float4(output.posH.xyz, 1.0f), view);
    output.posH = mul(float4(output.posH.xyz, 1.0f), proj);
    
    output.Tex = float2(0, 0);
    output.Nomal = mul(input.Nomal, (float3x3) view);
    output.Nomal = mul(output.Nomal, (float3x3) proj);
    
    output.Tangent = mul(input.Tangent, (float3x3) world);
    
    
    
   
    return output;
}