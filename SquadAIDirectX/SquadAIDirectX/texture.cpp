#include "texture.h"



texture::texture()
{
	m_texture = 0;
	m_textures[0] = 0;
	m_textures[1] = 0;
	m_textures[2] = 0;
}

texture::texture(const texture& other)
{
}


texture::~texture()
{
}

bool texture::Initialize(ID3D11Device* device, const WCHAR* filename, const WCHAR* filename1, const WCHAR* filename2)
{
	// Load the texture in.
	HRESULT result = CreateWICTextureFromFile(device, filename,&m_texture, &m_textures[0]);
	if (FAILED(result))
	{
		return false;
	}
	result = CreateWICTextureFromFile(device, filename1,&m_texture, &m_textures[1]);
	if (FAILED(result))
	{
		return false;
	}
	result = CreateWICTextureFromFile(device, filename2,&m_texture, &m_textures[2]);
	if (FAILED(result))
	{
		return false;
	}
	return true;
}

void texture::Shutdown()
{
	// Release the texture resource.
	if (m_textures[0])
	{
		m_textures[0]->Release();
		m_textures[0] = 0;
	}

	if (m_textures[1])
	{
		m_textures[1]->Release();
		m_textures[1] = 0;
	}

	if (m_textures[2])
	{
		m_textures[2]->Release();
		m_textures[2] = 0;
	}
	
	if (m_texture)
	{
		m_texture->Release();
		m_texture = 0;
	}

	return;
}

ID3D11ShaderResourceView** texture::GetTextureArray()
{
	return m_textures;
}