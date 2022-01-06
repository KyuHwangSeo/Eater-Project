struct DebugVertexIn
{
    float4 PosH : SV_POSITION;
    float4 Color : COLOR;
};

float4 Debug_PS(DebugVertexIn pin) : SV_TARGET
{
    return pin.Color;
}