Texture2D shaderTexure;
SamplerState SampleType;

struct PixelInputType {
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
};


float4 TexturePixelShader(PixelInputType input) : SV_TARGET {
	float4 texColor
		= shaderTexure.Sample(SampleType, input.tex);

return texColor;//float4(texColor.r, texColor.r, texColor.r, texColor.r);
}