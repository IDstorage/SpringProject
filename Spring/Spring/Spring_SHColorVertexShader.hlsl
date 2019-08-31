cbuffer MatrixBuffer {
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

struct VertexInput {
	float4 position : POSITION;
	float4 color : COLOR;
};

struct PixelInput {
	float4 position : SV_POSITION;
	float4 color : COLOR;
};


PixelInput ColorVertexShader(VertexInput input) {
	PixelInput result;

	// 적절한 행렬 계산을 위해 위치 벡터를 4 단위로 변경
	input.position.w = 1.0f;

	// 월드, 뷰, 투영 행렬에 대한 정점 위치 계산
	result.position = mul(input.position, worldMatrix);
	result.position = mul(result.position, viewMatrix);
	result.position = mul(result.position, projectionMatrix);

	result.color = input.color;

	return result;
}


//float4 main( float4 pos : POSITION ) : SV_POSITION
//{
//	return pos;
//}