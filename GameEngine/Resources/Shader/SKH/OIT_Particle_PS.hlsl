#include "OIT.hlsli"

cbuffer cbParticleOption
{
    float4 gColor : packoffset(c0);
};

RWStructuredBuffer<FLStaticNode> gPieceLinkBuffer : register(u1);
RWByteAddressBuffer gFirstOffsetBuffer : register(u2);

Texture2D gDiffuseMap : register(t0);
SamplerState gSamWrapLinear : register(s0);

struct VertexIn
{
    float4 PosH : SV_POSITION;
    float2 Tex : TEXCOORD;
};

[earlydepthstencil]
void OIT_Particle_PS(VertexIn pIn)
{
    float4 texColor = gDiffuseMap.Sample(gSamWrapLinear, pIn.Tex);
    
    clip(texColor.a - 0.1f);

    // µÎ ÇÈ¼¿À» °öÇÏ°í °¨¸¶(2.2f)¸¦ °öÇØÁØ´Ù..
    texColor.xyz *= 2.2f;
    texColor = texColor * gColor;
    texColor = saturate(texColor);
    
    uint pixelCount = gPieceLinkBuffer.IncrementCounter();
    
    uint2 vPos = (uint2) pIn.PosH.xy;
    uint startOffsetAddress = 4 * (gFrameWidth * vPos.y + vPos.x);
    uint oldStartOffset;
    gFirstOffsetBuffer.InterlockedExchange(
        startOffsetAddress, pixelCount, oldStartOffset);
    
    FLStaticNode node;
    node.Data.Color = PackColorFromFloat4(texColor);
    node.Data.Depth = pIn.PosH.z;
    node.Next = oldStartOffset;
    
    gPieceLinkBuffer[pixelCount] = node;
}
