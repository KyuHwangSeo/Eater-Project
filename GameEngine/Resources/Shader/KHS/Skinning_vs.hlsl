#include "MainHeader.hlsli"

struct VertexInputType
{
    uint4 BoneIndex00 : BONE00_INDEX;
    uint4 BoneIndex01 : BONE01_INDEX;
    
    float4 BoneWeights0 : BONE00_WEIGHT;
    float4 BoneWeights1 : BONE01_WEIGHT;
    
    float3 PosL : POSITION;
    float2 Tex : TEXCOORD;
    float3 Nomal : NORMAL;
    float3 Tangent : TANGENT;
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
    float weights[8] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
    uint Boneindex[8] ={ 0, 0, 0, 0, 0, 0, 0, 0 };
    
    weights[0] = input.BoneWeights0.x;
    weights[1] = input.BoneWeights0.y;
    weights[2] = input.BoneWeights0.z;
    weights[3] = input.BoneWeights0.w;
    weights[4] = input.BoneWeights1.x;
    weights[5] = input.BoneWeights1.y;
    weights[6] = input.BoneWeights1.z;
    weights[7] = input.BoneWeights1.w;
    
    Boneindex[0] = input.BoneIndex00.x;
    Boneindex[1] = input.BoneIndex00.y;
    Boneindex[2] = input.BoneIndex00.z;
    Boneindex[3] = input.BoneIndex00.w;
    Boneindex[4] = input.BoneIndex01.x;
    Boneindex[5] = input.BoneIndex01.y;
    Boneindex[6] = input.BoneIndex01.z;
    Boneindex[7] = input.BoneIndex01.w;

    float3 posL     = float3(0.0f, 0.0f, 0.0f);
    float3 normalL  = float3(0.0f, 0.0f, 0.0f);
    
    for (int i = 0; i < 8; i++)
    {
        int index = Boneindex[i];
        posL += weights[i] * mul(BoneOffset[index], float4(input.PosL, 1.0f)).xyzw;
        //normalL += weights[i] * mul(input.Nomal, (float3x3) BoneOffset[Boneindex[i]]);
    }
    
    

    
    output.posH = mul(float4(posL, 1.0f), Skinning_world);
    output.posH = mul(float4(output.posH.xyz, 1.0f), view);
    output.posH = mul(float4(output.posH.xyz, 1.0f), proj);
    
    output.Tex      = mul(float4(input.Tex.xy, 0.0f, 1.0f), Skinning_Tex);
    output.Nomal    = mul(input.Nomal, (float3x3) view);
    output.Nomal    = mul(output.Nomal, (float3x3) proj);
    
    output.Tangent = mul(input.Tangent, (float3x3) world);
    
    
   
    return output;
}