cbuffer cbDebugObject : register(b0)
{
    float4x4 gWorldViewProj : packoffset(c0);
};

struct DebugVertexIn
{
    float3 PosL : POSITION;
    float4 Color : COLOR;
};

struct DebugVertexOut
{
    float4 PosH : SV_POSITION;
    float4 Color : COLOR;
};

DebugVertexOut Debug_VS(DebugVertexIn vin)
{
    DebugVertexOut vout;

	// Transform to homogeneous clip space.
    vout.PosH = mul(gWorldViewProj, float4(vin.PosL, 1.0f));

	// Just pass vertex color into the pixel shader.
    vout.Color = vin.Color;

    return vout;
}