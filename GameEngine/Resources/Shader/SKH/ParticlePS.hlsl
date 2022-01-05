cbuffer cbParticleOption
{
    float4 gColor : packoffset(c0);
};

Texture2D gDiffuseMap : register(t1);

SamplerState gSamWrapLinear : register(s1);

struct ParticleVertexIn
{
    float4 PosH : SV_POSITION;
    float2 Tex : TEXCOORD;
};

float4 Particle_PS(ParticleVertexIn pin) : SV_TARGET
{
    float4 pout = gDiffuseMap.Sample(gSamWrapLinear, pin.Tex);
    
    clip(pout.a - 0.1f);
    
    // �� �ȼ��� ���ϰ� ����(2.2f)�� �����ش�..
    pout = pout * gColor * 2.2f;
    
    pout = saturate(pout);
    
    return pout;
}