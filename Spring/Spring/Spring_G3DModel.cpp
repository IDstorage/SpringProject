#include "Spring_G3DModel.h"

using namespace DirectX;
using namespace spring;


G3DModel::G3DModel() {}

G3DModel::G3DModel(const G3DModel& m) {}

G3DModel::~G3DModel() {}


bool G3DModel::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext) {
	vertexCount = 8;
	indexCount = 36;

	position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	eulerAngle = XMFLOAT3(0.0f, 45.0f, 0.0f);

	VertexType* vertices = new	VertexType[vertexCount];
	if (!vertices)
		return false;

	vertices[0] = { XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) };
	vertices[1] = { XMFLOAT3(-1.0f, +1.0f, -1.0f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f) };
	vertices[2] = { XMFLOAT3(+1.0f, +1.0f, -1.0f), XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f) };
	vertices[3] = { XMFLOAT3(+1.0f, -1.0f, -1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) };
	vertices[4] = { XMFLOAT3(-1.0f, -1.0f, +1.0f), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f) };
	vertices[5] = { XMFLOAT3(-1.0f, +1.0f, +1.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) };
	vertices[6] = { XMFLOAT3(+1.0f, +1.0f, +1.0f), XMFLOAT4(0.5f, 1.0f, 0.5f, 1.0f) };
	vertices[7] = { XMFLOAT3(+1.0f, -1.0f, +1.0f), XMFLOAT4(0.25f, 0.0f, 0.75f, 1.0f) };

	for (int i = 0; i < vertexCount; i++)
		vertices[i].position = XMFLOAT3(vertices[i].position.x + position.x, vertices[i].position.y + position.y, vertices[i].position.z + position.z);

	/*
	vertices[0] = { XMFLOAT3(-1.0f + position.x, -1.0f + position.y, -1.0f + position.z), XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f) };
	vertices[1] = { XMFLOAT3(-1.0f + position.x, +1.0f + position.y, -1.0f + position.z), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f) };
	vertices[2] = { XMFLOAT3(+1.0f + position.x, +1.0f + position.y, -1.0f + position.z), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) };
	vertices[3] = { XMFLOAT3(+1.0f + position.x, -1.0f + position.y, -1.0f + position.z), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) };
	vertices[4] = { XMFLOAT3(-1.0f + position.x, -1.0f + position.y, +1.0f + position.z), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f) };
	vertices[5] = { XMFLOAT3(-1.0f + position.x, +1.0f + position.y, +1.0f + position.z), XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f) };
	vertices[6] = { XMFLOAT3(+1.0f + position.x, +1.0f + position.y, +1.0f + position.z), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f) };
	vertices[7] = { XMFLOAT3(+1.0f + position.x, -1.0f + position.y, +1.0f + position.z), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) };*/

	unsigned long* indices = new unsigned long[36]{
		0, 1, 2,
		0, 2, 3,

		4, 6, 5,
		4, 7, 6,

		4, 5, 1,
		4, 1, 0,

		3, 2, 6,
		3, 6, 7,

		1, 5, 6,
		1, 6, 2,

		4, 0, 3,
		4, 3, 7
	};

	// 정적 버텍스 버퍼 구조체
	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// 버텍스 데이터를 가리키는 보조 리소스 구조체
	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	if (FAILED(device->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer)))
		return false;

	// 버텍스 인덱스 버퍼 구조체
	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// 인덱스 데이터를 가리키는 보조 리소스 구조체
	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	if (FAILED(device->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer)))
		return false;

	size_t stride = sizeof(VertexType);
	size_t offset = 0;

	if (!deviceContext)
		return false;

	// 렌더링 가능하도록 입력 어셈블리에서 버텍스 버퍼를 활성으로 설정
	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	// 렌더링 가능하도록 입력 어셈블리에서 인덱스 버퍼를 활성으로 설정
	deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	delete[] vertices;
	vertices = nullptr;

	delete[] indices;
	indices = nullptr;

	return true;
}

void G3DModel::Release() {
	if (indexBuffer) {
		indexBuffer->Release();
		indexBuffer = nullptr;
	}

	if (vertexBuffer) {
		vertexBuffer->Release();
		vertexBuffer = nullptr;
	}
}

void G3DModel::Render(ID3D11DeviceContext* deviceContext) {
	

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}


int G3DModel::GetIndexCount() const {
	return indexCount;
}


void G3DModel::SetEulerRotation(float x, float y, float z) {
	XMFLOAT3 up = XMFLOAT3(0.0f, 1.0f, 0.0f),
		pos = position,
		lookAt = XMFLOAT3(0.0f, 0.0f, 1.0f);
	XMVECTOR upVector, posVector, lookAtVector;

	float yaw, pitch, roll;
	XMMATRIX rotationMat;

	upVector = XMLoadFloat3(&up);
	posVector = XMLoadFloat3(&pos);
	lookAtVector = XMLoadFloat3(&lookAt);

	pitch = eulerAngle.x * FMath::Deg2Rad;
	yaw = eulerAngle.y * FMath::Deg2Rad;
	roll = eulerAngle.z * FMath::Deg2Rad;

	// 회전 행렬 생성
	rotationMat = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// lookAt 벡터와 up 벡터를 회전 행렬로 변형하여 뷰가 올바르게 원점에서 회전하도록 함
	lookAtVector = XMVector3TransformCoord(lookAtVector, rotationMat);
	upVector = XMVector3TransformCoord(upVector, rotationMat);

	// 회전된 카메라 위치를 뷰어 위치로 변환
	lookAtVector = XMVectorAdd(posVector, lookAtVector);

	// 세 벡터를 통해 뷰 행렬을 만듦
	viewMatrix = XMMatrixLookAtLH(posVector, lookAtVector, upVector);
}

void G3DModel::SetEulerRotationDX(XMFLOAT3 angles) {
	SetEulerRotation(angles.x, angles.y, angles.z);
}