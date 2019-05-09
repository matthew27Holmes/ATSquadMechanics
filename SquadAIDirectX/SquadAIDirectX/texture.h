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
	bool Initialize(ID3D11Device*, const WCHAR*);
	void Shutdown();
	ID3D11ShaderResourceView* GetTexture();

private:
	ID3D11ShaderResourceView* m_textureView;
	ID3D11Texture2D*		  m_texture;
	ID3D11SamplerState*       m_sampler;
};

