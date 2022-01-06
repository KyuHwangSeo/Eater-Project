float4 OIT_VS(float3 vPos : POSITION) : SV_Position
{
    return float4(vPos, 1.0f);
}