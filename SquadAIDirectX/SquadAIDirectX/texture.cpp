#include "texture.h"



texture::texture()
{
	m_texture = 0;
	m_textureView = 0;
	
}

texture::texture(const texture& other)
{
}


texture::~texture()
{
}

bool texture::Initialize(ID3D11Device* device, const WCHAR* filename)
{
	// Load the texture in.
	HRESULT result = CreateWICTextureFromFile(device, filename,&m_texture, &m_textureView);
	if (FAILED(result))
	{
		return false;
	}
	return true;
}

void texture::Shutdown()
{
	// Release the texture resource.
	if (m_textureView)
	{
		m_textureView->Release();
		m_textureView = 0;
	}
	
	if (m_texture)
	{
		m_texture->Release();
		m_texture = 0;
	}

	return;
}

ID3D11ShaderResourceView* texture::GetTexture()
{
	return m_textureView;
}

