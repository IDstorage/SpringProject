struct PixelInput {
	float4 position : SV_POSITION;
	float4 color : COLOR;
};


float4 ColorPixelShader(PixelInput input) : SV_TARGET{
	float avg = 0.0f;// (input.color.r + input.color.g + input.color.b) / 3.0f;
	return float4(avg, avg, avg, 1.0f);
}


//float4 main() : SV_TARGET
//{
//	return float4(1.0f, 1.0f, 1.0f, 1.0f);
//}