#include "Spring_G3DModel.h"

using namespace DirectX;
using namespace spring;


G3DModel::G3DModel() {}

G3DModel::G3DModel(const G3DModel& m) {}

G3DModel::~G3DModel() {}


bool G3DModel::Initialize(ID3D11Device* device) {
	vertexCount = 3;
	indexCount = 3;

	VertexType* vertices = new	VertexType[vertexCount];
	if (!vertices)
		return false;

	size_t* indices = new size_t[indexCount];
	if (!indices)
		return false;

	vertices[0].position = XMFLOAT4(-1.0f, -1.0f, 0.0f, 1.0f);
	vertices[0].color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);

	vertices[1].position = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	vertices[1].color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);

	vertices[2].position = XMFLOAT4(1.0f, -1.0f, 0.0f, 1.0f);
	vertices[2].color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;

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
	indexBufferDesc.ByteWidth = sizeof(unsigned long) / indexCount;
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
	size_t stride = sizeof(VertexType);
	size_t offset = 0;

	if (!deviceContext)
		return;
	
	// 렌더링 가능하도록 입력 어셈블리에서 버텍스 버퍼를 활성으로 설정
	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	// 렌더링 가능하도록 입력 어셈블리에서 인덱스 버퍼를 활성으로 설정
	deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}


int G3DModel::GetIndexCount() const {
	return indexCount;
}