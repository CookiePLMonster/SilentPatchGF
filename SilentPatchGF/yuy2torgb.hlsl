uniform Texture2D tex;
uniform SamplerState samp;

struct VS_OUTPUT
{
	float4 Position : POSITION0;
	float4 Color : COLOR0;
	float2 UV : TEXCOORD0;
};

float4 main(const VS_OUTPUT In) : COLOR0
{
	float3 ycbcr =
	float3( tex.Sample(samp, In.UV).r - 0.0625,
		tex.Sample(samp, In.UV).g - 0.5,
		tex.Sample(samp, In.UV).b - 0.5 );

	return float4( dot(float3(1.1644, 0.0, 1.7927), ycbcr), // R
		dot(float3(1.1644, -0.2133, -0.5329), ycbcr), // G
		dot(float3(1.1644, 2.1124, 0.0), ycbcr), // B
		1.0 ); // A
}