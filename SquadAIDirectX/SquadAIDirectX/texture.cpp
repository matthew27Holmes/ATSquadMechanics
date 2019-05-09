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
	HRESULT result = CreateWICTextureFromFile(device, filename,(ID3D11Resource**)&m_texture, &m_textureView);
	if (FAILED(result))
	{
		return false;
	}

	D3D11_SAMPLER_DESC  sampleDesc = {};
	sampleDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampleDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampleDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
	sampleDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampleDesc.MinLOD = 0.0f;
	sampleDesc.MaxLOD = D3D11_FLOAT32_MAX;

	result = device->CreateSamplerState(&sampleDesc, &m_sampler);
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

