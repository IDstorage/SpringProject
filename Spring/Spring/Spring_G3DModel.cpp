#include "Spring_G3DModel.h"
#include "Spring_FMath.h"

using namespace DirectX;
using namespace spring;


G3DModel::G3DModel() {}

G3DModel::G3DModel(const G3DModel& m) {}

G3DModel::~G3DModel() {}


bool G3DModel::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext) {

	this->device = device;
	this->deviceContext = deviceContext;

	vertexCount = 8;
	indexCount = 36;

	position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	eulerAngle = XMFLOAT3(0.0f, 0.0f, 0.0f); 

	lookAtVector = XMFLOAT3(0.0f, 0.0f, -1.0f);
	 
	vertexList[0] = XMFLOAT3(-1.0f, -1.0f, -1.0f);
	vertexList[1] = XMFLOAT3(-1.0f, +1.0f, -1.0f);
	vertexList[2] = XMFLOAT3(+1.0f, +1.0f, -1.0f);
	vertexList[3] = XMFLOAT3(+1.0f, -1.0f, -1.0f);
	vertexList[4] = XMFLOAT3(-1.0f, -1.0f, +1.0f);
	vertexList[5] = XMFLOAT3(-1.0f, +1.0f, +1.0f);
	vertexList[6] = XMFLOAT3(+1.0f, +1.0f, +1.0f);
	vertexList[7] = XMFLOAT3(+1.0f, -1.0f, +1.0f);

	VertexType* vertices = new VertexType[vertexCount];
	if (!vertices)
		return false;

	vertices[0] = { vertexList[0], XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) };
	vertices[1] = { vertexList[1], XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f) };
	vertices[2] = { vertexList[2], XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f) };
	vertices[3] = { vertexList[3], XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) };
	vertices[4] = { vertexList[4], XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f) };
	vertices[5] = { vertexList[5], XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) };
	vertices[6] = { vertexList[6], XMFLOAT4(0.5f, 1.0f, 0.5f, 1.0f) };
	vertices[7] = { vertexList[7], XMFLOAT4(0.25f, 0.0f, 0.75f, 1.0f) };

	for (int i = 0; i < vertexCount; i++) {
		vertices[i].position = XMFLOAT3(vertices[i].position.x + position.x, vertices[i].position.y + position.y, vertices[i].position.z + position.z);

		float basicRad = FMath::Sqrt(vertexList[i].x * vertexList[i].x + vertexList[i].y * vertexList[i].y + vertexList[i].z * vertexList[i].z);

		vertVectorList[i] = XMFLOAT3( 
			vertices[i].position.x - lookAtVector.x,
			vertices[i].position.y - lookAtVector.y,
			vertices[i].position.z - lookAtVector.z
		);

		float basicAngleY = FMath::ATan2(vertexList[i].z, vertexList[i].x);
		
		if ((vertexList[i].z * vertexList[i].x) > 0.0f && vertexList[i].y < 0.0f)
			basicRad *= -1;
		float basicAngleZ = FMath::ATan2(vertexList[i].y, basicRad);//FMath::ACos(vertexList[i].z / basicRad);
		vertexBasicAngle[i] = XMFLOAT3(basicRad, basicAngleY, basicAngleZ);
	}

	//basicLookRot = XMFLOAT3(0.0f, FMath::ATan2(lookAtVector.z, lookAtVector.x), FMath::ATan2(lookAtVector.y, lookAtVector.x));
	basicLookRot = XMFLOAT3(0.0f, 90.0f * FMath::Deg2Rad, 0.0f);

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



XMFLOAT3 G3DModel::RotateX(XMFLOAT3 pos, float angle) {
	return RotateZ(
		RotateY(
			RotateZ(pos, angle),
			angle
		),
		-angle
	);
}

XMFLOAT3 G3DModel::RotateY(XMFLOAT3 pos, float angle) {

	float r = FMath::Sqrt(pos.x * pos.x + pos.z * pos.z);

	return XMFLOAT3(
		r * FMath::Cos(angle),
		pos.y,
		r * FMath::Sin(angle)
	);
}

XMFLOAT3 G3DModel::RotateZ(XMFLOAT3 pos, float angle) {
	float r = FMath::Sqrt(pos.x * pos.x + pos.y * pos.y);

	return XMFLOAT3(
		r * FMath::Cos(angle),
		r * FMath::Sin(angle),
		pos.z
	);
}

XMFLOAT3 G3DModel::ChangeToSphericalCoord(int index) {  

	XMFLOAT3 yawPithRoll = XMFLOAT3(eulerAngle.y, eulerAngle.z, eulerAngle.x);

	float sinPith = FMath::Sin_Deg(yawPithRoll.x / 2), cosPith = FMath::Cos_Deg(yawPithRoll.x / 2);
	float sinYaw = FMath::Sin_Deg(yawPithRoll.y / 2), cosYaw = FMath::Cos_Deg(yawPithRoll.y / 2);
	float sinRoll = FMath::Sin_Deg(yawPithRoll.z / 2), cosRoll = FMath::Cos_Deg(yawPithRoll.z / 2);

	XMVECTOR q = XMLoadFloat4(new XMFLOAT4(
		sinRoll * cosPith * cosYaw - cosRoll * sinPith * sinYaw,
		cosRoll * sinPith * cosYaw + sinRoll * cosPith * sinYaw,
		cosRoll * cosPith * sinYaw - sinRoll * sinPith * cosYaw,
		cosRoll * cosPith * cosYaw + sinRoll * sinPith * sinYaw
	));

	XMFLOAT3 local;
	XMStoreFloat3(&local, XMVector3Rotate(XMLoadFloat3(&vertexList[index]), q));

	return local;

	//return RotateZ(vertexList[index], alphaZ);
	//return RotateZ(RotateY(vertexList[index], alphaY), alphaZ);
	//return RotateZ(RotateY(RotateX(vertexList[index], alphaX), alphaY), alphaZ);

	//float basicRad = FMath::Sqrt(vertexList[index].x * vertexList[index].x + vertexList[index].y * vertexList[index].y + vertexList[index].z * vertexList[index].z);
	//float newRad = basicRad * FMath::Sin_Deg(euler.z);

	//XMFLOAT3 spher = XMFLOAT3(
	//	basicRad,
	//	FMath::ATan2(pos.y, pos.x),
	//	FMath::ACos(pos.z / basicRad) 
	//);

	////spher.x += euler.x * FMath::Deg2Rad;
	//spher.y += euler.y * FMath::Deg2Rad;
	////spher.z += euler.z * FMath::Deg2Rad;

	//XMFLOAT3 local = XMFLOAT3(
	//	vertexList[index].x * FMath::Cos(alphaY) - vertexList[index].z * FMath::Sin(alphaY),// * FMath::Cos(spher.z),
	//	vertexList[index].y,//spher.x * FMath::Sin(spher.y) * FMath::Sin(spher.z),
	//	vertexList[index].x * FMath::Sin(alphaY) + vertexList[index].z * FMath::Cos(alphaY)// * FMath::Sin(alphaY)// * FMath::Sin(spher.z)
	//); 

 //	return local;
}

void G3DModel::SetEulerAngle(float x, float y, float z) {
	eulerAngle.x = x;
	eulerAngle.y = y;
	eulerAngle.z = z;

	XMFLOAT3 yawPithRoll = XMFLOAT3(eulerAngle.y, eulerAngle.z, eulerAngle.x);

	//lookAtVector = RotateZ(RotateY(lookAtVector, eulerAngle.y * FMath::Deg2Rad - basicLookRot.y), eulerAngle.z * FMath::Deg2Rad - basicLookRot.z);
	float sinPith = FMath::Sin_Deg(yawPithRoll.x / 2), cosPith = FMath::Cos_Deg(yawPithRoll.x / 2);
	float sinYaw = FMath::Sin_Deg(yawPithRoll.y / 2), cosYaw = FMath::Cos_Deg(yawPithRoll.y / 2);
	float sinRoll = FMath::Sin_Deg(yawPithRoll.z / 2), cosRoll = FMath::Cos_Deg(yawPithRoll.z / 2);

	XMVECTOR q = XMLoadFloat4(new XMFLOAT4(
		sinRoll * cosPith * cosYaw - cosRoll * sinPith * sinYaw,
		cosRoll * sinPith * cosYaw + sinRoll * cosPith * sinYaw,
		cosRoll * cosPith * sinYaw - sinRoll * sinPith * cosYaw,
		cosRoll * cosPith * cosYaw + sinRoll * sinPith * sinYaw
	));

	XMStoreFloat3(&lookAtVector, XMVector3Rotate(XMLoadFloat3(&lookAtVector), q));

	VertexType* vertices = new	VertexType[vertexCount];
	if (!vertices)
		return;

	vertices[0] = { ChangeToSphericalCoord(0), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) };
	vertices[1] = { ChangeToSphericalCoord(1), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f) };
	vertices[2] = { ChangeToSphericalCoord(2), XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f) };
	vertices[3] = { ChangeToSphericalCoord(3), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) };
	vertices[4] = { ChangeToSphericalCoord(4), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f) };
	vertices[5] = { ChangeToSphericalCoord(5), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) };
	vertices[6] = { ChangeToSphericalCoord(6), XMFLOAT4(0.5f, 1.0f, 0.5f, 1.0f) };
	vertices[7] = { ChangeToSphericalCoord(7), XMFLOAT4(0.25f, 0.0f, 0.75f, 1.0f) };

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
		return;

	delete[] vertices;
	vertices = nullptr;
}

void G3DModel::SetEulerAngle(XMFLOAT3 angles) {
	SetEulerAngle(angles.x, angles.y, angles.z);
}


XMFLOAT3 G3DModel::GetEulerAngle() const {
	return eulerAngle;
}