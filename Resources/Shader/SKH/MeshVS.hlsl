cbuffer cbPerObject : register(b0)
{
    float4x4 gWorld : packoffset(c0);
    float4x4 gWorldViewProj : packoffset(c4);
    float4x4 gTexTransform : packoffset(c8);
};

struct VertexIn
{
    float3 PosL     : POSITION;
    float3 NormalL  : NORMAL;
    float2 Tex      : TEXCOORD;
    float3 TangentL : TANGENT;
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

	// 세계 공간 변환
    vout.PosW = mul(gWorldViewProj, float4(vin.PosL, 1.0f));

    vout.NormalW = mul((float3x3)gWorld, vin.NormalL);
    vout.NormalW = normalize(vout.NormalW);
	
	// Output vertex attributes for interpolation across triangle.
    vout.Tex = mul(gTexTransform, float4(vin.Tex, 0.0f, 1.0f)).xy;
    
    float3 TangentW = mul((float3x3) gWorld, vin.TangentL);
    
    // Vertex Shader 에서 TBN을 구해주자..
	// Pixel Shader에서 연산은 최소한으로 해야하기 때문..
    float3 N = vout.NormalW;
    float3 T = normalize(TangentW - dot(TangentW, N) * N);
    float3 B = cross(N, T);
    
    vout.TBN = float3x3(T, B, N);
    
    return vout;
}