#include "Header.hlsl"

PixelInputType VS(VertexInputType input)
{
    PixelInputType output;
    
    input.position.w = 1.0f;
    
	output.position = input.position;
    
    output.position = mul(input.position, world);   
    //output.position = mul(input.position, view);
    //output.position = mul(input.position, proj);
   

	output.color = input.position;
	output.uv = input.uv;
    return output;
}