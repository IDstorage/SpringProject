Texture2D shaderTex;
SamplerState sampleType;

struct PixelInputType {
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
};


float4 TexturePixelShader(PixelInputType input) : SV_TARGET{
	float4 texColor
		= shaderTex.Sample(sampleType, input.tex);

	return texColor;
}