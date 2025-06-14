#include <iostream>
#include "VertexShader.h"
#include "GraphicsEngine.h"


VertexShader::VertexShader()
{

}

void VertexShader::release()
{ 
	this->m_vs->Release();
	if (this)
		delete this;
}

bool VertexShader::init(const void* shader_byte_code, size_t byte_code_size)
{
	if(SUCCEEDED(GraphicsEngine::get()->m_d3d_device->CreateVertexShader(shader_byte_code, byte_code_size, nullptr, &this->m_vs)))
		return true;

	std::cout << "Error: V.Shader \n";
	return false;
}

VertexShader::~VertexShader()
{
}