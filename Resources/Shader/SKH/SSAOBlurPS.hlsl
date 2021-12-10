cbuffer cbBlurOption : register(b0)
{
    float2 gTexelSize	: packoffset(c0.x);
    bool gHorizon		: packoffset(c0.z);
};

static float gWeights[11] = { 0.05f, 0.05f, 0.1f, 0.1f, 0.1f, 0.2f, 0.1f, 0.1f, 0.1f, 0.05f, 0.05f };
static int gBlurRadius = 5;

Texture2D gDepthMap	: register(t0);
Texture2D gInputMap	: register(t1);

SamplerState samClampMinLinearPoint : register(s0);

struct VertexIn
{
	float4 PosH  : SV_POSITION;
	float2 Tex   : TEXCOORD;
};

float4 main(VertexIn pin) : SV_Target
{
    float2 texOffset = float2(0.0f, 0.0f);
	
    if (gHorizon)
    {
        texOffset = float2(gTexelSize.x, 0.0f);
    }
    else
    {
        texOffset = float2(0.0f, gTexelSize.y);
    }

	// The center value always contributes to the sum.
    float4 color = gWeights[gBlurRadius] * gInputMap.SampleLevel(samClampMinLinearPoint, pin.Tex, 0.0);
    float totalWeight = gWeights[gBlurRadius];

    float4 centerNormalDepth = gDepthMap.SampleLevel(samClampMinLinearPoint, pin.Tex, 0.0f);

	for (float i = -gBlurRadius; i <= gBlurRadius; ++i)
	{
		// We already added in the center weight.
		if (i == 0)
			continue;

		float2 tex = pin.Tex + i * texOffset;

        float4 neighborNormalDepth = gDepthMap.SampleLevel(samClampMinLinearPoint, tex, 0.0f);

		//
		// If the center value and neighbor values differ too much (either in 
		// normal or depth), then we assume we are sampling across a discontinuity.
		// We discard such samples from the blur.
		//

		if (dot(neighborNormalDepth.xyz, centerNormalDepth.xyz) >= 0.8f &&
			abs(neighborNormalDepth.a - centerNormalDepth.a) <= 0.2f)
		{
			float weight = gWeights[i + gBlurRadius];

			// Add neighbor pixel to blur.
			color += weight * gInputMap.SampleLevel(
				samClampMinLinearPoint, tex, 0.0);

			totalWeight += weight;
		}
	}

	// Compensate for discarded samples by making total weights sum to 1.
	return color / totalWeight;
}