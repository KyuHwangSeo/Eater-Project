cbuffer cbShadowMeshObject : register(b0)
{
    float4x4 gWorldViewProj : packoffset(c0);
};

struct VertexIn
{
    float3 PosL : POSITION;
};

float4 main(VertexIn vin) : SV_POSITION
{
    return mul(gWorldViewProj, float4(vin.PosL, 1.0f));
};