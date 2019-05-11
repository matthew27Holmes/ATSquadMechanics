#pragma once
#include <d3d11.h>
#include "WICTextureLoader.h"

using namespace DirectX;

class texture
{
public:
	texture();
	texture(const texture&);
	~texture();
	bool Initialize(ID3D11Device*, const WCHAR*, const WCHAR*, const WCHAR*);
	void Shutdown();
	ID3D11ShaderResourceView** GetTextureArray();

private:
	ID3D11ShaderResourceView * m_textures[3];
	ID3D11Resource*		  m_texture;
	ID3D11SamplerState*       m_sampler;
};

