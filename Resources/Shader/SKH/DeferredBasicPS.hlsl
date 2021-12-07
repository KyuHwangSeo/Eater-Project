cbuffer cbMaterial : register(b0)
{
    float4 gColor : packoffset(c0);
    int gMatID : packoffset(c1.x);
};

struct VertexIn
{
    float4 PosH : SV_POSITION;
    float3 PosW : POSITIONW;
    float3 PosV : POSITIONV;
    float3 NormalW : NORMALW;
    float3 NormalV : NORMALV;
    float2 Tex : TEXCOORD;
    float3 ShadowPosH : POS_SHADOW;
    float3x3 TBN : TANGENT;
};

struct PixelOut
{
    float4 Albedo       : SV_Target0;
    float4 Normal       : SV_Target1;
    float4 Position     : SV_Target2;
    float4 Shadow       : SV_Target3;
    float4 NormalDepth  : SV_Target4;
};

PixelOut main(VertexIn pin)
{
	PixelOut vout;

    vout.Albedo = gColor;
    vout.Normal = float4(pin.NormalW, 1.0f);
    vout.Position = float4(pin.PosW, gMatID);
    vout.Shadow = float4(pin.ShadowPosH.xyz, 0.0f);
    vout.NormalDepth = float4(pin.NormalV.xyz, pin.PosV.z);
    
	return vout;
}