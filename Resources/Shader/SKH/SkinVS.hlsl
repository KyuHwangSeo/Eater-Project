cbuffer cbSkinObject : register(b0)
{
    float4x4 gWorld                 : packoffset(c0);
    float4x4 gWorldView             : packoffset(c4);
    float4x4 gWorldViewProj         : packoffset(c8);
    float4x4 gTexTransform          : packoffset(c12);
    float4x4 gShadowTransform       : packoffset(c16);
    float4x4 gBoneTransforms[96]    : packoffset(c20);
};

struct VertexIn
{
    uint4 BoneIndices1 : BONEINDICES1;
    uint4 BoneIndices2 : BONEINDICES2;
    float4 BoneWeights1 : WEIGHTS1;
    float4 BoneWeights2 : WEIGHTS2;
    
    float3 PosL : POSITION;
    float2 Tex : TEXCOORD;
    float3 NormalL : NORMAL;
    float3 TangentL : TANGENT;
};

struct VertexOut
{
    float4 PosH : SV_POSITION;
    float3 PosW : POSITIONW;
    float3 PosV : POSITIONV;
    float3 NormalW : NORMALW;
    float3 NormalV : NORMALV;
    float2 Tex : TEXCOORD;
    float3 ShadowPosH : POS_SHADOW;
    
    float3x3 TBNW : TANGENTW;
    float3x3 TBNV : TANGENTV;
};

VertexOut main(VertexIn vin)
{
    VertexOut vout;

    float3 posL = float3(0.0f, 0.0f, 0.0f);
    float3 normalL = float3(0.0f, 0.0f, 0.0f);
    float3 tangentL = float3(0.0f, 0.0f, 0.0f);
    
    for (int i = 0; i < 4; ++i)
    {
        posL += vin.BoneWeights1[i] * mul( gBoneTransforms[vin.BoneIndices1[i]], float4(vin.PosL, 1.0f)).xyz;
        normalL += vin.BoneWeights1[i] * mul((float3x3) gBoneTransforms[vin.BoneIndices1[i]], vin.NormalL);
        tangentL += vin.BoneWeights1[i] * mul((float3x3) gBoneTransforms[vin.BoneIndices1[i]], vin.TangentL);
    }
    
    for (int j = 0; j < 4; ++j)
    {
        posL += vin.BoneWeights2[j] * mul( gBoneTransforms[vin.BoneIndices2[j]], float4(vin.PosL, 1.0f)).xyz;
        normalL += vin.BoneWeights2[j] * mul((float3x3) gBoneTransforms[vin.BoneIndices2[j]], vin.NormalL);
        tangentL += vin.BoneWeights2[j] * mul((float3x3) gBoneTransforms[vin.BoneIndices2[j]], vin.TangentL);
    }
    
    // 동차 공간 변환
    vout.PosH = mul(gWorldViewProj, float4(posL, 1.0f));
	
    // 세계 공간 변환
    vout.PosW = mul(gWorld, float4(posL, 1.0f)).xyz;

    // 뷰 공간 변환
    vout.PosV = mul(gWorldView, float4(posL, 1.0f)).xyz;
    
    vout.NormalW = mul((float3x3) gWorld, normalL);
    vout.NormalW = normalize(vout.NormalW);
	
    vout.NormalV = mul((float3x3) gWorldView, normalL);
    vout.NormalV = normalize(vout.NormalV);
    
	// Output vertex attributes for interpolation across triangle.
    vout.Tex = mul(gTexTransform, float4(vin.Tex, 0.0f, 1.0f)).xy;
    
    float4 Shadow = mul(gShadowTransform, float4(posL, 1.0f));
    vout.ShadowPosH = Shadow.xyz / Shadow.w;
    
    // Vertex Shader 에서 TBN을 구해주자..
	// Pixel Shader에서 연산은 최소한으로 해야하기 때문..
    float3 Tangent = float3(0.0f, 0.0f, 0.0f);
    float3 N = float3(0.0f, 0.0f, 0.0f);
    float3 T = float3(0.0f, 0.0f, 0.0f);
    float3 B = float3(0.0f, 0.0f, 0.0f);
    
    Tangent = mul((float3x3) gWorld, tangentL);
    N = vout.NormalW;
    T = normalize(Tangent - dot(Tangent, N) * N);
    B = cross(N, T);
    
    vout.TBNW = float3x3(T, B, N);
    
    Tangent = mul((float3x3) gWorldView, tangentL);
    N = vout.NormalV;
    T = normalize(Tangent - dot(Tangent, N) * N);
    B = cross(N, T);

    vout.TBNV = float3x3(T, B, N);
    
    return vout;
}