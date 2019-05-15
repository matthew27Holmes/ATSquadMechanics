#include "texture.h"



texture::texture()
{
	m_texture = 0;
	m_textures[0] = 0;
	m_textures[1] = 0;
	m_textures[2] = 0;
	m_textures[3] = 0;
	m_textures[4] = 0;
	m_textures[5] = 0;
	m_textures[6] = 0;
}

texture::texture(const texture& other)
{
}


texture::~texture()
{
}

bool texture::Initialize(ID3D11Device* device, const WCHAR* filename, const WCHAR* filename1, const WCHAR* filename2, const WCHAR* filename3, const WCHAR* filename4, const WCHAR* filename5, const WCHAR* filename6)
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
	result = CreateWICTextureFromFile(device, filename3,&m_texture, &m_textures[3]);
	if (FAILED(result))
	{
		return false;
	}
	result = CreateWICTextureFromFile(device, filename4,&m_texture, &m_textures[4]);
	if (FAILED(result))
	{
		return false;
	}
	result = CreateWICTextureFromFile(device, filename5,&m_texture, &m_textures[5]);
	if (FAILED(result))
	{
		return false;
	}
	result = CreateWICTextureFromFile(device, filename6,&m_texture, &m_textures[6]);
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
	
	if (m_textures[3])
	{
		m_textures[3]->Release();
		m_textures[3] = 0;
	}
	if (m_textures[4])
	{
		m_textures[4]->Release();
		m_textures[4] = 0;
	}
	if (m_textures[5])
	{
		m_textures[5]->Release();
		m_textures[5] = 0;
	}
	if (m_textures[6])
	{
		m_textures[6]->Release();
		m_textures[6] = 0;
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