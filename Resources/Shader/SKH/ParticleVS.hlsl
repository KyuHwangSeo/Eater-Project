cbuffer cbParticleObject
{
    float4x4 gWorldViewProj : packoffset(c0);
    float4x4 gTexTransform : packoffset(c4);
};

struct ParticleVertexIn
{
    float3 PosL : POSITION;
    float2 Tex : TEXCOORD;
};

struct ParticleVertexOut
{
    float4 PosH : SV_POSITION;
    float2 Tex : TEXCOORD;
};

ParticleVertexOut Particle_VS(ParticleVertexIn vin)
{
    ParticleVertexOut vout;
    
    vout.PosH = mul(gWorldViewProj, float4(vin.PosL, 1.0f));
    vout.Tex = mul(gTexTransform, float4(vin.Tex, 0.0f, 1.0f)).xy;
    
    return vout;
}