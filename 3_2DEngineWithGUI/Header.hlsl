
Texture2D gTextureMaps[48] : register(t0);

SamplerState gSamPointWrap        : register(s0);
SamplerState gSamPointClamp       : register(s1);
SamplerState gSamLinearWrap       : register(s2);
SamplerState gSamLinearClamp      : register(s3);
SamplerState gSamAnisotropicWrap  : register(s4);
SamplerState gSamAnisotropicClamp : register(s5);
SamplerComparisonState gSamShadow : register(s6);

// Constant data that varies per frame.
cbuffer cbTransformPerObjects : register(b0)
{	

};

cbuffer cbTransformInCamera : register(b1)
{

};

cbuffer cbRenderingInformation : register(b2)
{

};

struct VertexInputType
{
	float4 position : POSITION;
	float4 normal   : NORMAL;
	float4 tangent  : TANGENT;
	float2 uv       : TEXCOORD;
}; // ok

struct PixelInputType
{
	float4 position : SV_POSITION;
	float4 color    : COLOR;
	float2 uv       : TEXCOORD;
};
