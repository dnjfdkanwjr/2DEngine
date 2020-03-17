#include "Header.hlsl"

PixelInputType VS(VertexInputType input)
{
    PixelInputType output;
    
    input.position.w = 1.0f;
    

	output.position = input.position;
    output.position = mul(output.position, view );
    output.position = mul(output.position, proj );
	output.position = mul(output.position, world);
	

	output.color = input.position;
	output.uv = input.uv;
    return output;
}