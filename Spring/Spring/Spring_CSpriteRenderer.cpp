#include "Spring_CSpriteRenderer.h"
#include "Spring_GRenderSystem.h"

using namespace DirectX;
using namespace spring;


CSpriteRenderer::CSpriteRenderer() {}

CSpriteRenderer::CSpriteRenderer(const CSpriteRenderer&) {}

CSpriteRenderer::~CSpriteRenderer() {}


bool CSpriteRenderer::Initialize(const std::string& fileName, const FSize& bitmapSize) {

	device = Renderer->GetD3DClass()->GetDevice();
	deviceContext = Renderer->GetD3DClass()->GetDeviceContext();

	size = bitmapSize;

	vertexCount = indexCount = 6;

	VertexType* vertices = new VertexType[vertexCount];
	if (!vertices)
		return false;

	memset(vertices, 0, sizeof(VertexType) * vertexCount);

	unsigned long* indices = new unsigned long[indexCount];
	if (!indices)
		return false;

	for (int i = 0; i < indexCount; i++)
		indices[i] = i;

	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	if (FAILED(device->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer)))
		return false;

	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

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

void CSpriteRenderer::Render() {

}

void CSpriteRenderer::Release() {
	if (indexBuffer) {
		indexBuffer->Release();
		indexBuffer = nullptr;
	}

	if (vertexBuffer) {
		vertexBuffer->Release();
		vertexBuffer = nullptr;
	}
}