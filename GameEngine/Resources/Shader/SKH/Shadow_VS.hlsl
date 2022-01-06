cbuffer cbShadowMeshObject : register(b0)
{
    float4x4 gMeshWorldViewProj : packoffset(c0);
};

cbuffer cbShadowSkinObject : register(b0)
{
    float4x4 gSkinWorldViewProj : packoffset(c0);
    float4x4 gSkinBoneTransforms[96] : packoffset(c4);
};

struct MeshVertexIn
{
    float3 PosL : POSITION;
};

struct SkinVertexIn
{
    uint4 BoneIndices1 : BONEINDICES1;
    uint4 BoneIndices2 : BONEINDICES2;
    float4 BoneWeights1 : WEIGHTS1;
    float4 BoneWeights2 : WEIGHTS2;
    
    float3 PosL : POSITION;
};

float4 MeshShadow_VS(MeshVertexIn vin) : SV_POSITION
{
    return mul(gMeshWorldViewProj, float4(vin.PosL, 1.0f));
};

float4 SkinShadow_VS(SkinVertexIn vin) : SV_POSITION
{
    float3 posL = float3(0.0f, 0.0f, 0.0f);

    for (int i = 0; i < 4; ++i)
    {
        posL += vin.BoneWeights1[i] * mul(gSkinBoneTransforms[vin.BoneIndices1[i]], float4(vin.PosL, 1.0f)).xyz;
    }
	
    for (int j = 0; j < 4; ++j)
    {
        posL += vin.BoneWeights2[j] * mul(gSkinBoneTransforms[vin.BoneIndices2[j]], float4(vin.PosL, 1.0f)).xyz;
    }
	
    
    return mul(gSkinWorldViewProj, float4(posL, 1.0f));
};