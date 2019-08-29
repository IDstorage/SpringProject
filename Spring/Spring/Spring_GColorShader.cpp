#include "Spring_GColorShader.h"

using namespace DirectX;
using namespace spring;


GColorShader* GColorShader::instance = nullptr;

GColorShader::GColorShader()
	: vertexShader(nullptr), pixelShader(nullptr),
		inputLayout(nullptr), matrixBuffer(nullptr) {}


bool GColorShader::Initialize(ID3D11Device* device, HWND hWnd) {

	const WCHAR* vsFilePath = L"./Spring_SHColorVertexShader.hlsl",
		* psFilePath = L".ne/Spring_SHColorPixelShader.hlsl";

	ID3D10Blob* errorMsg = nullptr;

	// ���ؽ� ���̴� ������
	ID3D10Blob* vertShBuffer = nullptr;
	D3D_SHADER_MACRO* shaderMacro = new D3D_SHADER_MACRO{ "ColorVertexShader" };
	if (FAILED(D3DCompile(vsFilePath, NULL, NULL, shaderMacro, (ID3DInclude*)(UINT_PTR)1,
		(LPCSTR)NULL, (LPCSTR)D3D10_SHADER_ENABLE_STRICTNESS, 0, 0, &vertShBuffer, &errorMsg))) {

		if (errorMsg) {
			MessageBox(hWnd, reinterpret_cast<LPCTSTR>(errorMsg->GetBufferPointer()), L"Error while compile vertex shader", MB_OK);
			errorMsg->Release();
			errorMsg = nullptr;
		}
		else
			MessageBox(hWnd, L"No File Exist.", vsFilePath, MB_OK);

		return false;
	}

	// �ȼ� ���̴� ������
	ID3D10Blob* pixelShBuffer = nullptr;
	shaderMacro = new D3D_SHADER_MACRO{ "ColorPixelShader" };
	if (FAILED(D3DCompile(psFilePath, NULL, NULL, shaderMacro, (ID3DInclude*)(UINT_PTR)1,
		(LPCSTR)NULL, (LPCSTR)D3D10_SHADER_ENABLE_STRICTNESS, 0, 0, &pixelShBuffer, &errorMsg))) {

		if (errorMsg) {
			MessageBox(hWnd, reinterpret_cast<LPCTSTR>(errorMsg->GetBufferPointer()), L"Error while compile pixel shader", MB_OK);
			errorMsg->Release();
			errorMsg = nullptr;
		}
		else
			MessageBox(hWnd, L"No File Exist.", vsFilePath, MB_OK);

		return false;
	}

	// ���۷κ��� ���ؽ� ���̴� ����
	if (FAILED(device->CreateVertexShader(vertShBuffer->GetBufferPointer(), vertShBuffer->GetBufferSize(), NULL, &instance->vertexShader)))
		return false;

	// ���۷κ��� �ȼ� ���̴� ����
	if (FAILED(device->CreatePixelShader(pixelShBuffer->GetBufferPointer(), pixelShBuffer->GetBufferSize(), NULL, &instance->pixelShader)))
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

	if (FAILED(device->CreateInputLayout(polygonLayout, numElements, vertShBuffer->GetBufferPointer(), vertShBuffer->GetBufferSize(), &instance->inputLayout)))
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

	if (FAILED(device->CreateBuffer(&matrixBufferDesc, NULL, &instance->matrixBuffer)))
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

	ReleaseFunc(instance->matrixBuffer);

	ReleaseFunc(instance->inputLayout);

	ReleaseFunc(instance->pixelShader);

	ReleaseFunc(instance->vertexShader);

}


bool GColorShader::SetShaderParameters(ID3D11DeviceContext* deviceContext, XMMATRIX worldMat, XMMATRIX viewMat, XMMATRIX projectionMat) {
	// ����� Transpose�Ͽ� ���̴����� ��밡���ϵ��� ��
	worldMat = XMMatrixTranspose(worldMat);
	viewMat = XMMatrixTranspose(viewMat);
	projectionMat = XMMatrixTranspose(projectionMat);

	// ��� ������ ������ �� �� �ֵ��� ���
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	if (FAILED(deviceContext->Map(instance->matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
		return false;

	// ��� ������ �����Ϳ� ���� ������
	MatrixBufferType* dataPtr = (MatrixBufferType*)mappedResource.pData;

	// ��� ���ۿ� ��� ����
	dataPtr->world = worldMat;
	dataPtr->view = viewMat;
	dataPtr->projection = projectionMat;

	// ��� ������ ����� ǰ
	deviceContext->Unmap(instance->matrixBuffer, 0);

	// ���� ���̴������� ��� ���� ��ġ ����
	unsigned bufferNumber = 0;

	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &instance->matrixBuffer);

	return true;
}

bool GColorShader::Render(ID3D11DeviceContext* deviceContext, XMMATRIX worldMat, XMMATRIX viewMat, XMMATRIX projectionMat, int indexCount) {

	if (!instance->SetShaderParameters(deviceContext, worldMat, viewMat, projectionMat))
		return false;

	// ���� �Է� ���̾ƿ� ����
	deviceContext->IASetInputLayout(instance->inputLayout);

	// ���̴� ����
	deviceContext->VSSetShader(instance->vertexShader, NULL, 0);
	deviceContext->PSSetShader(instance->pixelShader, NULL, 0);

	// �׸���
	deviceContext->DrawIndexed(indexCount, 0, 0);

	return true;
}