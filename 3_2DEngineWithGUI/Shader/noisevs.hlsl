#include "Header.hlsl"

PixelInputType VS(VertexInputType input)
{
    PixelInputType output;
    
    input.position.w = 1.0f;
    

	output.position = input.position;
    //output.position = mul(output.position, view );
    //output.position = mul(output.position, proj );
   
    //float2 newUv = input.position.xy;
    //
    //newUv.x += 0.05;
    //newUv.y += 0.05;
    //
    //float4 ns = texNoise.SampleLevel(samplerLinear, newUv, 0);
    //output.position *= ns;
	output.color = input.position;
	output.uv = input.uv;
    return output;
}