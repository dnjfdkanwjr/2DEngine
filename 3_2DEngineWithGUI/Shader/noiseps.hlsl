#include "Header.hlsl"


float4 PS(PixelInputType input) : SV_TARGET
{
    float4 returnCol = input.color;
   

    return returnCol;

}

