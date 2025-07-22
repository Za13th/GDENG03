#pragma once
#include "Resource.h"
#include <d3d11.h>

class DeviceContext;
class Texture : public Resource
{
public:
	Texture(const wchar_t* full_path);
	~Texture();

	ID3D11Resource* getTexture() { return m_texture; }

private:
	ID3D11Resource* m_texture = nullptr;
	ID3D11ShaderResourceView* m_shader_resource_view = nullptr;

	friend class DeviceContext;
};

