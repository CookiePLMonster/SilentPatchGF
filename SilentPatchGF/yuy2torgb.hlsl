sampler2D tex : register(s0);

struct VS_OUTPUT
{
	float4 Position : POSITION0;
	float4 Color : COLOR0;
	float2 UV : TEXCOORD0;
};

float4 main(VS_OUTPUT In) : COLOR0
{
	return tex2D(tex, In.UV);
}