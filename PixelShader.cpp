#include <iostream>
#include "PixelShader.h"
#include "GraphicsEngine.h"

PixelShader::PixelShader()
{

}

void PixelShader::release()
{
	this->m_ps->Release();
	if (this)
		delete this;
}

bool PixelShader::init(const void* shader_byte_code, size_t byte_code_size)
{
	if (SUCCEEDED(GraphicsEngine::get()->m_d3d_device->CreatePixelShader(shader_byte_code, byte_code_size, nullptr, &this->m_ps)))
		return true;

	std::cout << "Error: P.Shader \n";
	return false;
}

PixelShader::~PixelShader()
{

}