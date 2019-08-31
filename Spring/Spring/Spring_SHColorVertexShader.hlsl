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

	// ������ ��� ����� ���� ��ġ ���͸� 4 ������ ����
	input.position.w = 1.0f;

	// ����, ��, ���� ��Ŀ� ���� ���� ��ġ ���
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