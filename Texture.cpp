#include "Texture.h"
#include <iostream>
#include <DirectXTex.h>
#include "GraphicsEngine.h"

Texture::Texture(const wchar_t* full_path) : Resource(full_path)
{
	DirectX::ScratchImage image_data;
	HRESULT res = DirectX::LoadFromWICFile(full_path, DirectX::WIC_FLAGS_NONE, nullptr, image_data);

	if (SUCCEEDED(res))
	{
		res = DirectX::CreateTexture(GraphicsEngine::get()->getDevice(),image_data.GetImages(),
			image_data.GetImageCount(),image_data.GetMetadata(), &m_texture);

		D3D11_SHADER_RESOURCE_VIEW_DESC desc = {};
		desc.Format = image_data.GetMetadata().format;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipLevels = image_data.GetMetadata().mipLevels;
		desc.Texture2D.MostDetailedMip = 0;

		GraphicsEngine::get()->getDevice()->CreateShaderResourceView(m_texture, &desc, &m_shader_resource_view);
	}
	else
	{
		throw::std::exception("Texture Creation Failed!");
	}
}

Texture::~Texture()
{
	if(m_texture)
		m_texture->Release();
	if (m_shader_resource_view)
		m_shader_resource_view->Release();
}
