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

	// 버텍스 쉐이더 컴파일
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

	// 픽셀 쉐이더 컴파일
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

	// 버퍼로부터 버텍스 쉐이더 생성
	if (FAILED(device->CreateVertexShader(vertShBuffer->GetBufferPointer(), vertShBuffer->GetBufferSize(), NULL, &instance->vertexShader)))
		return false;

	// 버퍼로부터 픽셀 쉐이더 생성
	if (FAILED(device->CreatePixelShader(pixelShBuffer->GetBufferPointer(), pixelShBuffer->GetBufferSize(), NULL, &instance->pixelShader)))
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


}