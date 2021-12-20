cbuffer cbParticleObject
{
    float4 gColor : packoffset(c0);
};

Texture2D gDiffuseMap : register(t0);

SamplerState gSamWrapLinear : register(s1);

struct ParticleVertexIn
{
    float4 PosH : SV_POSITION;
    float2 Tex : TEXCOORD;
};

float4 Particle_PS(ParticleVertexIn pin)
{
    float4 pout = gDiffuseMap.Sample(gSamWrapLinear, pin.Tex);
    
    pout = pout * gColor;
    
    return pout;
}