cbuffer cbParticleOption
{
    float4 gColor : packoffset(c0);
};

Texture2D gDiffuseMap : register(t1);

SamplerState gSamWrapLinear : register(s1);

struct ParticleVertexIn
{
    float4 PosH : SV_POSITION;
    float4 PosW : POSITIONW;
    float2 Tex : TEXCOORD;
};

float4 Particle_PS(ParticleVertexIn pin) : SV_TARGET
{
    float4 pout = gDiffuseMap.Sample(gSamWrapLinear, pin.Tex);
    
    clip(pout.a - 0.05f);
    
    //pout.a *= 1.5f;
    pout = 2.0f * pout * gColor;
    
    pout.a = saturate(pout.a);
    
    return pout;
}