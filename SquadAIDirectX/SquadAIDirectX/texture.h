#pragma once
#include "DXApp.h"

class texture
{
public:
	texture();
	texture(const texture&);
	~texture();
	bool Initialize(ID3D11Device*, WCHAR*);
	void Shutdown();
	ID3D11ShaderResourceView* GetTexture();

private:
	ID3D11ShaderResourceView * m_texture;
};

