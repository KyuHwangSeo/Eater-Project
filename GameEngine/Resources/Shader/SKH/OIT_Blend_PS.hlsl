#include "OIT.hlsli"

StructuredBuffer<FLStaticNode> gPieceLinkBuffer : register(t0);
ByteAddressBuffer gFirstOffsetBuffer : register(t1);
Texture2D gBackGround : register(t2);

#define MAX_SORTED_PIXELS 16

static FragmentData gSortedPixels[MAX_SORTED_PIXELS];

void SortPixelInPlace(int numPixels)
{
    FragmentData temp;
    for (int i = 1; i < numPixels; ++i)
    {
        for (int j = i - 1; j >= 0; --j)
        {
            if (gSortedPixels[j].Depth < gSortedPixels[j + 1].Depth)
            {
                temp = gSortedPixels[j];
                gSortedPixels[j] = gSortedPixels[j + 1];
                gSortedPixels[j + 1] = temp;
            }
            else
            {
                break;
            }
        }
    }
}

float4 OIT_PS(float4 posH : SV_Position) : SV_Target
{
    uint2 vPos = (uint2) posH.xy;
    int startOffsetAddress = 4 * (gFrameWidth * vPos.y + vPos.x);
    int numPixels = 0;
    uint offset = gFirstOffsetBuffer.Load(startOffsetAddress);
    
    FLStaticNode element;
    
    while (offset != 0xFFFFFFFF)
    {
        element = gPieceLinkBuffer[offset];
        gSortedPixels[numPixels++] = element.Data;
        offset = (numPixels >= MAX_SORTED_PIXELS) ?
            0xFFFFFFFF : element.Next;
    }
    
    SortPixelInPlace(numPixels);
    
    float4 currColor = gBackGround.Load(int3(posH.xy, 0));
    
    for (int i = 0; i < numPixels; ++i)
    {
        float4 pixelColor = UnpackColorFromUInt(gSortedPixels[i].Color);
        currColor.xyz = lerp(currColor.xyz, pixelColor.xyz, pixelColor.w);
    }
    
    return currColor;
}