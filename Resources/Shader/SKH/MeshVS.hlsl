#pragma pack_matrix(row_major)

cbuffer cbMeshObject : register(b0)
{
    float4x4 gWorld             : packoffset(c0);
    float4x4 gWorldViewProj     : packoffset(c4);
    float4x4 gTexTransform      : packoffset(c8);
    float4x4 gShadowTransform   : packoffset(c12);
};

struct VertexIn
{
    float3 PosL     : POSITION;
    float2 Tex      : TEXCOORD;
    float3 NormalL  : NORMAL;
    float3 TangentL : TANGENT;
};

struct VertexOut
{
    float4 PosH : SV_POSITION;
    float3 PosW : POSITIONW;
    float2 Tex : TEXCOORD;
    float3 NormalW : NORMALW;
    float3 ShadowPosH : POS_SHADOW;
    
    float3x3 TBN : TANGENT;
};

VertexOut main(VertexIn vin)
{
    VertexOut vout;

    // 동차 공간 변환
    vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj);
    
	// 세계 공간 변환
    vout.PosW = mul(float4(vin.PosL, 1.0f), gWorld).xyz;

    vout.NormalW = mul(vin.NormalL, (float3x3) gWorld);
    vout.NormalW = normalize(vout.NormalW);
	
	// Output vertex attributes for interpolation across triangle.
    vout.Tex = mul(float4(vin.Tex, 0.0f, 1.0f), gTexTransform).xy;
    
    float3 TangentW = mul(vin.TangentL, (float3x3)gWorld);
    
    // Vertex Shader 에서 TBN을 구해주자..
	// Pixel Shader에서 연산은 최소한으로 해야하기 때문..
    float3 N = vout.NormalW;
    float3 T = normalize(TangentW - dot(TangentW, N) * N);
    float3 B = cross(N, T);
    
    vout.TBN = float3x3(T, B, N);
    
    float4 Shadow = mul(float4(vin.PosL, 1.0f), gShadowTransform);
    
    vout.ShadowPosH = Shadow.xyz / Shadow.w;
    
    return vout;
}