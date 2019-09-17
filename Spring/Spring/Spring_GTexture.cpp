#include "Spring_GTexture.h"
#include <fstream>

using namespace DirectX;
using namespace spring;


GTexture::GTexture() {}

GTexture::GTexture(const GTexture& t) {}

GTexture::~GTexture() {}


bool GTexture::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* fileName) {

	int width = 0, height = 0;

	// targa 이미지 데이터를 메모리에 로드
	if (!LoadTarga(fileName, width, height))
		return false;

	D3D11_TEXTURE2D_DESC textureDesc;
	textureDesc.Height = height;
	textureDesc.Width = width;
	textureDesc.MipLevels = 0;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	// 빈 텍스쳐 생성
	if (FAILED(device->CreateTexture2D(&textureDesc, NULL, &texture)))
		return false;

	// targa 이미지 데이터의 너비 사이즈 설정
	UINT rowPitch = (width * 4) * sizeof(unsigned char);

	// targa 이미지 데이터를 텍스쳐에 복사
	deviceContext->UpdateSubresource(texture, 0, NULL, targaData, rowPitch, 0);

	// 쉐이더 리소스 뷰 구조체
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;

	// 텍스쳐의 쉐이더 리소스 뷰 생성
	if (FAILED(device->CreateShaderResourceView(texture, &srvDesc, &textureView)))
		return false;

	deviceContext->GenerateMips(textureView);

	delete[] targaData;
	targaData = nullptr;

	return true;
}

void GTexture::Release() {
	
	if (textureView) {
		textureView->Release();
		textureView = nullptr;
	}

	if (texture) {
		texture->Release();
		texture = nullptr;
	}

	if (targaData) {
		delete[] targaData;
		targaData = nullptr;
	}

}


ID3D11ShaderResourceView* GTexture::GetTexture() const {
	return textureView;
}


bool GTexture::LoadTarga(char* fileName, int& width, int& height) {
	
	FILE* file;
	if (fopen_s(&file, fileName, "rb") != 0)
		return false;

	TargaHeader targaFileHeader;
	UINT count = (UINT)fread(&targaFileHeader, sizeof(TargaHeader), 1, file);
	if (count != 1)
		return false;

	width = (int)targaFileHeader.width;
	height = (int)targaFileHeader.height;
	int bpp = (int)targaFileHeader.bpp;

	// 파일이 32bit인지 24bit인지 확인
	if (bpp != 32)
		return false;

	int imageSize = width * height * 4;
	unsigned char* targaImage = new unsigned char[imageSize];
	if (!targaImage)
		return false;

	count = (UINT)fread(targaImage, 1, imageSize, file);
	if (count != imageSize)
		return false;

	if (fclose(file) != 0)
		return false;

	targaData = new unsigned char[imageSize];
	if (!targaData)
		return false;

	int index = 0;
	int k = (width * height * 4) - (width * 4);

	// targa 형식이 거꾸로 저장되었으므로 올바른 순서로 복사
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			targaData[index + 0] = targaImage[k + 2];
			targaData[index + 1] = targaImage[k + 1];
			targaData[index + 2] = targaImage[k + 0];
			targaData[index + 3] = targaImage[k + 3];

			k += 4;
			index += 4;
		}

		k -= (width * 8);
	}

	delete[] targaImage;
	targaImage = nullptr;

	return true;
}