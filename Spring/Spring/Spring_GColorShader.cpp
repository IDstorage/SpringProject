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

	// ���ؽ� ���̴� ������
	ID3D10Blob* vertShBuffer = nullptr;
	if (FAILED(D3DCompileFromFile(vsFilePath, NULL, NULL, "ColorVertexShader", "vs_5_0",
		D3D10_SHADER_ENABLE_STRICTNESS, 0, &vertShBuffer, &errorMsg))) {

		if (errorMsg)
			ShowErrorMsg(errorMsg, vsFilePath);
		else
			MessageBox(hWnd, L"No File Exist.", vsFilePath, MB_OK);

		return false;
	}

	// �ȼ� ���̴� ������
	ID3D10Blob* pixelShBuffer = nullptr;
	if (FAILED(D3DCompileFromFile(psFilePath, NULL, NULL, "ColorPixelShader", "ps_5_0",
		D3D10_SHADER_ENABLE_STRICTNESS, 0, &pixelShBuffer, &errorMsg))) {

		if (errorMsg)
			ShowErrorMsg(errorMsg, psFilePath);
		else
			MessageBox(hWnd, L"No File Exist.", vsFilePath, MB_OK);

		return false;
	}

	// ���۷κ��� ���ؽ� ���̴� ����
	if (FAILED(device->CreateVertexShader(vertShBuffer->GetBufferPointer(), vertShBuffer->GetBufferSize(), NULL, &vertexShader)))
		return false;

	// ���۷κ��� �ȼ� ���̴� ����
	if (FAILED(device->CreatePixelShader(pixelShBuffer->GetBufferPointer(), pixelShBuffer->GetBufferSize(), NULL, &pixelShader)))
		return false;

	// ���� ���̾ƿ� ����ü
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

	// ���̾ƿ� ����� ��
	size_t numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	if (FAILED(device->CreateInputLayout(polygonLayout, numElements, vertShBuffer->GetBufferPointer(), vertShBuffer->GetBufferSize(), &inputLayout)))
		return false;

	vertShBuffer->Release();
	vertShBuffer = nullptr;

	pixelShBuffer->Release();
	pixelShBuffer = nullptr;

	// ���� ���̴��� �ִ� ��� ��� ������ ����ü �ۼ�
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
	// ����� Transpose�Ͽ� ���̴����� ��밡���ϵ��� ��
	worldMat = XMMatrixTranspose(worldMat);
	viewMat = XMMatrixTranspose(viewMat);
	projectionMat = XMMatrixTranspose(projectionMat);

	// ��� ������ ������ �� �� �ֵ��� ���
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	if (FAILED(deviceContext->Map(matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
		return false;

	// ��� ������ �����Ϳ� ���� ������
	MatrixBufferType* dataPtr = (MatrixBufferType*)mappedResource.pData;

	// ��� ���ۿ� ��� ����
	dataPtr->world = worldMat;
	dataPtr->view = viewMat;
	dataPtr->projection = projectionMat;

	// ��� ������ ����� ǰ
	deviceContext->Unmap(matrixBuffer, 0);

	// ���� ���̴������� ��� ���� ��ġ ����
	unsigned bufferNumber = 0;

	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &matrixBuffer);

	return true;
}

bool GColorShader::Render(XMMATRIX worldMat, XMMATRIX viewMat, XMMATRIX projectionMat, int indexCount) {

	if (!SetShaderParameters(worldMat, viewMat, projectionMat))
		return false;

	// ���� �Է� ���̾ƿ� ����
	deviceContext->IASetInputLayout(inputLayout);

	// ���̴� ����
	deviceContext->VSSetShader(vertexShader, NULL, 0);
	deviceContext->PSSetShader(pixelShader, NULL, 0);

	// �׸���
	deviceContext->DrawIndexed(indexCount, 0, 0);

	return true;
}