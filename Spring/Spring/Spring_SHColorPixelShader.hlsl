struct PixelInput {
	float4 position : SV_POSITION;
	float4 color : COLOR;
};


float4 ColorPixelShader(PixelInput input) : SV_TARGET {
	return input.color;
}


//float4 main() : SV_TARGET
//{
//	return float4(1.0f, 1.0f, 1.0f, 1.0f);
//}