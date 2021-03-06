cbuffer cbSsaoFrustum : register(b0)
{
	float4   gFrustumCorners[4];
};

struct VertexIn
{
	float3 PosL            : POSITION;
	float2 Tex             : TEXCOORD;
	float3 ToFarPlaneIndex : NORMAL;
};

struct VertexOut
{
	float4 PosH       : SV_POSITION;
	float3 ToFarPlane : TEXCOORD0;
	float2 Tex        : TEXCOORD1;
};

VertexOut SSAO_VS(VertexIn vin)
{
	VertexOut vout;

	// Already in NDC space.
	vout.PosH = float4(vin.PosL, 1.0f);

	// We store the index to the frustum corner in the normal x-coord slot.
	vout.ToFarPlane = gFrustumCorners[vin.ToFarPlaneIndex.x].xyz;

	// Pass onto pixel shader.
	vout.Tex = vin.Tex;

	return vout;
}