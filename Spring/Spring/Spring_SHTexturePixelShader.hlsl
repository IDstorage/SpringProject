Texture2D shaderTexure;
SamplerState sampleType;

struct PixelInputType {
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
};


float4 TexturePixelShader(PixelInputType input) : SV_TARGET {
	float4 texColor
		= shaderTexure.Sample(sampleType, input.tex);

	return texColor;// float4(1.0f, 1.0f, 0.0f, 1.0f);
}