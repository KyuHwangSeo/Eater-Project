#pragma pack_matrix(row_major)
    
cbuffer cbShadowObject : register(b0)
{
    float4x4 gWorldViewProj : packoffset(c0);
};

struct VertexIn
{
    float3 PosL : POSITION;
};

float4 main(VertexIn vin) : SV_POSITION
{
    return mul(float4(vin.PosL, 1.0f), gWorldViewProj);
};