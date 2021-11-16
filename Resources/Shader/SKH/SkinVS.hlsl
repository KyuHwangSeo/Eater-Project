cbuffer cbPerObject : register(b0)
{
    float4x4 gWorld : packoffset(c0);
    float4x4 gWorldViewProj : packoffset(c4);
    float4x4 gTexTransform : packoffset(c8);
};

cbuffer cbSkinned : register(b3)
{
    float4x4 gBoneTransforms[96];
};

struct VertexIn
{
    float3 PosL : POSITION;
    float3 NormalL : NORMAL;
    float2 Tex : TEXCOORD;
    float3 TangentL : TANGENT;
    
    uint4 BoneIndices1 : BONEINDICES1;
    uint4 BoneIndices2 : BONEINDICES2;
    float4 BoneWeights1 : WEIGHTS1;
    float4 BoneWeights2 : WEIGHTS2;
};

struct VertexOut
{
    float4 PosW : SV_POSITION;
    float3 NormalW : NORMALW;
    float2 Tex : TEXCOORD;
    
    float3x3 TBN : TANGENT;
};

VertexOut main(VertexIn vin)
{
    VertexOut vout;

    float3 posL = float3(0.0f, 0.0f, 0.0f);
    float3 normalL = float3(0.0f, 0.0f, 0.0f);
    float3 tangentL = float3(0.0f, 0.0f, 0.0f);
    
    for (int i = 0; i < 4; ++i)
    {
        posL += vin.BoneWeights1[i] * mul(gBoneTransforms[vin.BoneIndices1[i]], float4(vin.PosL, 1.0f)).xyz;
        normalL += vin.BoneWeights1[i] * mul((float3x3) gBoneTransforms[vin.BoneIndices1[i]], vin.NormalL);
        tangentL += vin.BoneWeights1[i] * mul((float3x3) gBoneTransforms[vin.BoneIndices1[i]], vin.TangentL);
    }
    
    for (int j = 0; j < 4; ++j)
    {
        posL += vin.BoneWeights2[j] * mul(gBoneTransforms[vin.BoneIndices2[j]], float4(vin.PosL, 1.0f)).xyz;
        normalL += vin.BoneWeights2[j] * mul((float3x3) gBoneTransforms[vin.BoneIndices2[j]], vin.NormalL);
        tangentL += vin.BoneWeights2[j] * mul((float3x3) gBoneTransforms[vin.BoneIndices2[j]], vin.TangentL);
    }
    
	// 세계 공간 변환
    vout.PosW = mul(gWorldViewProj, float4(posL, 1.0f));

    vout.NormalW = mul((float3x3)gWorld, normalL);
    vout.NormalW = normalize(vout.NormalW);
	
	// Output vertex attributes for interpolation across triangle.
    vout.Tex = mul(gTexTransform, float4(vin.Tex, 0.0f, 1.0f)).xy;
    
    float3 TangentW = mul((float3x3)gWorld, tangentL);

    // Vertex Shader 에서 TBN을 구해주자..
	// Pixel Shader에서 연산은 최소한으로 해야하기 때문..
    float3 N = vout.NormalW;
    float3 T = normalize(TangentW - dot(TangentW, N) * N);
    float3 B = cross(N, T);
    
    vout.TBN = float3x3(T, B, N);
    
    return vout;
}