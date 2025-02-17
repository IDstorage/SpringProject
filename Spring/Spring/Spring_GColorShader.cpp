#include "Spring_GColorShader.h"

using namespace DirectX;
using namespace spring;


GColorShader* GColorShader::instance = nullptr;

GColorShader::GColorShader()
	: vertexShader(nullptr), pixelShader(nullptr),
		inputLayout(nullptr), matrixBuffer(nullptr),
		deviceContext(nullptr) {}


bool GColorShader::Initialize(ID3D11Device* device, ID3D11DeviceContext* context, HWND hWnd) {

	auto ShowErrorMsg = [&](ID3D10Blob* error, LPCWCHAR fileName) {
		LPVOID msg = error->GetBufferPointer();

		OutputDebugStringA(reinterpret_cast<const char*>(msg));

		MessageBox(hWnd, L"Error while compile vertex shader", fileName, MB_OK);

		error->Release();
		error = nullptr;
	};


	if (instance == nullptr)
		instance = new GColorShader();

	deviceContext = context;

	const WCHAR* vsFilePath = L"./Spring_SHColorVertexShader.hlsl",
		* psFilePath = L"./Spring_SHColorPixelShader.hlsl";

	ID3D10Blob* errorMsg = nullptr;

	// 버텍스 쉐이더 컴파일
	ID3D10Blob* vertShBuffer = nullptr;
	if (FAILED(D3DCompileFromFile(vsFilePath, NULL, NULL, "ColorVertexShader", "vs_5_0",
		D3D10_SHADER_ENABLE_STRICTNESS, 0, &vertShBuffer, &errorMsg))) {

		if (errorMsg)
			ShowErrorMsg(errorMsg, vsFilePath);
		else
			MessageBox(hWnd, L"No File Exist.", vsFilePath, MB_OK);

		return false;
	}

	// 픽셀 쉐이더 컴파일
	ID3D10Blob* pixelShBuffer = nullptr;
	if (FAILED(D3DCompileFromFile(psFilePath, NULL, NULL, "ColorPixelShader", "ps_5_0",
		D3D10_SHADER_ENABLE_STRICTNESS, 0, &pixelShBuffer, &errorMsg))) {

		if (errorMsg)
			ShowErrorMsg(errorMsg, psFilePath);
		else
			MessageBox(hWnd, L"No File Exist.", vsFilePath, MB_OK);

		return false;
	}

	// 버퍼로부터 버텍스 쉐이더 생성
	if (FAILED(device->CreateVertexShader(vertShBuffer->GetBufferPointer(), vertShBuffer->GetBufferSize(), NULL, &vertexShader)))
		return false;

	// 버퍼로부터 픽셀 쉐이더 생성
	if (FAILED(device->CreatePixelShader(pixelShBuffer->GetBufferPointer(), pixelShBuffer->GetBufferSize(), NULL, &pixelShader)))
		return false;

	// 정점 레이아웃 구조체
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "COLOR";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	// 레이아웃 요소의 수
	size_t numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	if (FAILED(device->CreateInputLayout(polygonLayout, numElements, vertShBuffer->GetBufferPointer(), vertShBuffer->GetBufferSize(), &inputLayout)))
		return false;

	vertShBuffer->Release();
	vertShBuffer = nullptr;

	pixelShBuffer->Release();
	pixelShBuffer = nullptr;

	// 정점 쉐이더에 있는 행렬 상수 버퍼의 구조체 작성
	D3D11_BUFFER_DESC matrixBufferDesc;
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	if (FAILED(device->CreateBuffer(&matrixBufferDesc, NULL, &matrixBuffer)))
		return false;

	return true;
}

void GColorShader::ShutdownShader() {

	auto ReleaseFunc = [&](IUnknown* target) {
		if (target) {
			target->Release();
			target = nullptr;
		}
	};

	ReleaseFunc(matrixBuffer);

	ReleaseFunc(inputLayout);

	ReleaseFunc(pixelShader);

	ReleaseFunc(vertexShader);

}


bool GColorShader::SetShaderParameters(XMMATRIX worldMat, XMMATRIX viewMat, XMMATRIX projectionMat) {
	// 행렬을 Transpose하여 쉐이더에서 사용가능하도록 함
	worldMat = XMMatrixTranspose(worldMat);
	viewMat = XMMatrixTranspose(viewMat);
	projectionMat = XMMatrixTranspose(projectionMat);

	// 상수 버퍼의 내용을 쓸 수 있도록 잠금
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	if (FAILED(deviceContext->Map(matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
		return false;

	// 상수 버퍼의 데이터에 대한 포인터
	MatrixBufferType* dataPtr = (MatrixBufferType*)mappedResource.pData;

	// 상수 버퍼에 행렬 복사
	dataPtr->world = worldMat;
	dataPtr->view = viewMat;
	dataPtr->projection = projectionMat;

	// 상수 버퍼의 잠금을 품
	deviceContext->Unmap(matrixBuffer, 0);

	// 정점 쉐이더에서의 상수 버퍼 위치 설정
	unsigned bufferNumber = 0;

	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &matrixBuffer);

	return true;
}

bool GColorShader::Render(XMMATRIX worldMat, XMMATRIX viewMat, XMMATRIX projectionMat, int indexCount) {

	if (!SetShaderParameters(worldMat, viewMat, projectionMat))
		return false;

	// 정점 입력 레이아웃 설정
	deviceContext->IASetInputLayout(inputLayout);

	// 쉐이더 설정
	deviceContext->VSSetShader(vertexShader, NULL, 0);
	deviceContext->PSSetShader(pixelShader, NULL, 0);

	// 그리기
	deviceContext->DrawIndexed(indexCount, 0, 0);

	return true;
}