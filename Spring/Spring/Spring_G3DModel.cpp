#include "Spring_G3DModel.h"
#include "Spring_FMath.h"
#include "Spring_GRenderSystem.h"

using namespace DirectX;
using namespace spring;


G3DModel::G3DModel() {}

G3DModel::G3DModel(const G3DModel& m) {}

G3DModel::~G3DModel() {}


bool G3DModel::Initialize(const G3DModel::VertexListType& vList, const G3DModel::IndexListType& indexList, char* fileName) {

	this->device = Renderer->GetD3DClass()->GetDevice();
	this->deviceContext = Renderer->GetD3DClass()->GetDeviceContext();

	vertexCount = vList.size();
	indexCount = indexList.size();

	position = FVector3(0.0f, 0.0f, 0.0f);
	eulerAngle = FVector3(0.0f, 0.0f, 0.0f);
	localScale = FVector3(1.0f, 1.0f, 1.0f);

	lookAtVector = FVector3(0.0f, 0.0f, -1.0f);

	vertexList = vList;

	VertexType* vertices = new VertexType[vertexCount];
	if (!vertices)
		return false;

	for (int i = 0; i < vertexCount; i++) {
		vertices[i] = *(vertexList.begin() + i);

		vertices[i].position =
			XMFLOAT3(vertices[i].position.x + position.x,
				vertices[i].position.y + position.y,
				vertices[i].position.z + position.z);
				
				
		//FVector3(vertices[i].position.x + position.x, vertices[i].position.y + position.y, vertices[i].position.z + position.z);

		//float basicRad = vertices[i].position.Magnitude();

		//vertVectorList[i] = XMFLOAT3( 
		//	vertices[i].position.x - lookAtVector.x,
		//	vertices[i].position.y - lookAtVector.y,
		//	vertices[i].position.z - lookAtVector.z
		//);

		//float basicAngleY = FMath::ATan2(vertexList[i].z, vertexList[i].x);
		//
		//if ((vertexList[i].z * vertexList[i].x) > 0.0f && vertexList[i].y < 0.0f)
		//	basicRad *= -1;
		//float basicAngleZ = FMath::ATan2(vertexList[i].y, basicRad);//FMath::ACos(vertexList[i].z / basicRad);
		//vertexBasicAngle[i] = XMFLOAT3(basicRad, basicAngleY, basicAngleZ);
	}

	//basicLookRot = XMFLOAT3(0.0f, FMath::ATan2(lookAtVector.z, lookAtVector.x), FMath::ATan2(lookAtVector.y, lookAtVector.x));
	//basicLookRot = XMFLOAT3(0.0f, 90.0f * FMath::Deg2Rad, 0.0f);

	unsigned long* indices = new unsigned long[indexCount];/*{
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
	};*/
	for (int i = 0; i < indexCount; i++)
		indices[i] = *(indexList.begin() + i);

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

	return LoadTexture(fileName);
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

void G3DModel::Render() {
	

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}


bool G3DModel::LoadTexture(char* fileName) {
	targetTexture = new GTexture();
	if (!targetTexture)
		return false;

	return targetTexture->Initialize(device, deviceContext, fileName);
}

void G3DModel::ReleaseTexture() {
	targetTexture->Release();
}


int G3DModel::GetIndexCount() const {
	return indexCount;
}

ID3D11ShaderResourceView* G3DModel::GetTexture() const {
	return targetTexture->GetTexture();
}


bool G3DModel::UpdateVertexBuffer() {
	VertexType* vertices = new	VertexType[vertexCount];
	if (!vertices)
		return false;

	for (int i = 0; i < vertexCount; i++) {
		// Scale
		vertices[i].position = GetScaleVector(FVector3(vertexList[i].position)).DXFloat3();
		// Rotation
		vertices[i].position = GetRotationVector(FVector3(vertices[i].position)).DXFloat3();
		// Translation
		//vertices[i].position += position;
	}

	size_t stride = sizeof(VertexType);
	size_t offset = 0;

	// 렌더링 가능하도록 입력 어셈블리에서 버텍스 버퍼를 활성으로 설정
	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

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

	delete[] vertices;
	vertices = nullptr;
	
	return true;
}


FVector3 G3DModel::GetRotationVector(const FVector3& v) {
	FQuaternion quat = FQuaternion::Euler(v);

	XMFLOAT3 local;
	XMStoreFloat3(&local, XMVector3Rotate(XMLoadFloat3(&v.DXFloat3()), quat.DXVector()));

	return FVector3(local.x, local.y, local.z);
}

void G3DModel::SetEulerAngle(float x, float y, float z) {
	eulerAngle.x = x;
	eulerAngle.y = y;
	eulerAngle.z = z;

	FVector3 yawPithRoll = FVector3(eulerAngle.y, eulerAngle.z, eulerAngle.x);

	FQuaternion quat = FQuaternion::Euler(yawPithRoll);

	/*float sinPith = FMath::Sin_Deg(yawPithRoll.x / 2), cosPith = FMath::Cos_Deg(yawPithRoll.x / 2);
	float sinYaw = FMath::Sin_Deg(yawPithRoll.y / 2), cosYaw = FMath::Cos_Deg(yawPithRoll.y / 2);
	float sinRoll = FMath::Sin_Deg(yawPithRoll.z / 2), cosRoll = FMath::Cos_Deg(yawPithRoll.z / 2);*/

	/*XMVECTOR q = XMLoadFloat4(new XMFLOAT4(
		sinRoll * cosPith * cosYaw - cosRoll * sinPith * sinYaw,
		cosRoll * sinPith * cosYaw + sinRoll * cosPith * sinYaw,
		cosRoll * cosPith * sinYaw - sinRoll * sinPith * cosYaw,
		cosRoll * cosPith * cosYaw + sinRoll * sinPith * sinYaw
	));*/

	XMStoreFloat3(&lookAtVector.DXFloat3(), XMVector3Rotate(XMLoadFloat3(&lookAtVector.DXFloat3()), quat.DXVector()));

	UpdateVertexBuffer();
}

void G3DModel::SetEulerAngle(const FVector3& angles) {
	SetEulerAngle(angles.x, angles.y, angles.z);
}

void G3DModel::SetEulerAngle(XMFLOAT3 angles) {
	SetEulerAngle(angles.x, angles.y, angles.z);
}

FEulerAngle G3DModel::GetEulerAngle() const {
	return eulerAngle;
}


FVector3 G3DModel::GetScaleVector(const FVector3& v) {

	FVector3 targetPos = v;
	FVector3 dir = (targetPos - position).Normalized();

	dir *= localScale;

	return position + dir;
}

void G3DModel::SetLocalScale(float x, float y, float z) {
	localScale.x = x;
	localScale.y = y;
	localScale.z = z;

	UpdateVertexBuffer();
}