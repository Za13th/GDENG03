#include <iostream>
#include "IndexBuffer.h"
#include "GraphicsEngine.h"

IndexBuffer::IndexBuffer() : m_buffer(0)
{

}

bool IndexBuffer::load(void* list_vertices, UINT size_list)
{
	if (this->m_buffer)this->m_buffer->Release();

	D3D11_BUFFER_DESC buff_desc = {};
	buff_desc.Usage = D3D11_USAGE_DEFAULT;
	buff_desc.ByteWidth = size_list * 4;
	buff_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	buff_desc.CPUAccessFlags = 0;
	buff_desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA init_data = {};
	init_data.pSysMem = list_vertices;

	m_size_list = size_list;

	if (FAILED(GraphicsEngine::get()->m_d3d_device->CreateBuffer(&buff_desc, &init_data, &this->m_buffer)))
	{
		std::cout << "Error: I.Buffer \n";
		return false;
	}

	return true;
}

UINT IndexBuffer::getSizeIndexList()
{
	return this->m_size_list;
}
bool IndexBuffer::release()
{
	this->m_buffer->Release();
	delete this;
	return true;
}
IndexBuffer::~IndexBuffer()

{

}