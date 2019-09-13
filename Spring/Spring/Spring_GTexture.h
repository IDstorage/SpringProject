#pragma once

#include "Spring_GDX11Header.h"


namespace spring {

	class GTexture {

		struct TargaHeader {
			unsigned char data1[12];
			unsigned short width, height;
			unsigned char bpp;
			unsigned char data2;
		};

	private:
		unsigned char* targaData;
		ID3D11Texture2D* texture;
		ID3D11ShaderResourceView* textureView;

	public:
		GTexture();
		GTexture(const GTexture&);
		~GTexture();

	public:
		bool Initialize(ID3D11Device*, ID3D11DeviceContext*, char*);
		void Release();

	public:
		ID3D11ShaderResourceView* GetTexture() const;

	private:
		bool LoadTarga(char*, int&, int&);
	};

}