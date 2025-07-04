#include <iostream>
#include "GraphicsEngine.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"

GraphicsEngine* GraphicsEngine::sharedInstance = nullptr;

GraphicsEngine* GraphicsEngine::getInstance()
{
	if (!sharedInstance)
	{
		sharedInstance = new GraphicsEngine();
		sharedInstance->init();
	}
	return sharedInstance;
}

void GraphicsEngine::initialize()
{
	sharedInstance = new GraphicsEngine();
	sharedInstance->init();
}

void GraphicsEngine::destroy()
{
	if (sharedInstance != NULL)
		sharedInstance->release();
}

GraphicsEngine::GraphicsEngine()
{

}


bool GraphicsEngine::init()
{
	D3D_DRIVER_TYPE driver_types[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE
	};

	UINT num_driver_types = ARRAYSIZE(driver_types);

	D3D_FEATURE_LEVEL feature_levels[] =
	{
		D3D_FEATURE_LEVEL_11_0
	};

	UINT num_feature_levels = ARRAYSIZE(feature_levels);


	HRESULT res = 0;
	ID3D11DeviceContext* m_imm_context;
	for (UINT driver_type_index = 0; driver_type_index < num_driver_types;)
	{
		res = D3D11CreateDevice(NULL, driver_types[driver_type_index], NULL, NULL, feature_levels, num_feature_levels,
			D3D11_SDK_VERSION, &m_d3d_device, &m_feature_level, &m_imm_context);

		if (SUCCEEDED(res))
			break;
		++driver_type_index;
	}

	if (FAILED(res))
	{
		std::cout << "G.Engine FAIL 1\n";
		return false;
	}

	this->m_imm_device_context = new DeviceContext(m_imm_context);

	this->m_d3d_device->QueryInterface(__uuidof(IDXGIDevice), (void**)&m_dxgi_device);
	this->m_dxgi_device->GetParent(__uuidof(IDXGIAdapter), (void**)&m_dxgi_adapter);
	this->m_dxgi_adapter->GetParent(__uuidof(IDXGIFactory), (void**)&m_dxgi_factory);


	return true;
}

bool GraphicsEngine::release()
{
	this->m_dxgi_device->Release();
	this->m_dxgi_adapter->Release();
	this->m_dxgi_factory->Release();

	this->m_imm_device_context->release();
	this->m_d3d_device->Release();
	return true;
}

GraphicsEngine::~GraphicsEngine()
{

}

SwapChain* GraphicsEngine::createSwapChain()
{
	return new SwapChain();
}

DeviceContext* GraphicsEngine::getDeviceContext()
{
	return this->m_imm_device_context;
}

VertexBuffer* GraphicsEngine::createVertexBuffer()
{
	return new VertexBuffer();
}

IndexBuffer* GraphicsEngine::createIndexBuffer()
{
	return new IndexBuffer();
}

ConstantBuffer* GraphicsEngine::createConstantBuffer()
{
	return new ConstantBuffer();
}

VertexShader* GraphicsEngine::createVertexShader(const void* shader_byte_code, size_t byte_code_size)
{
	VertexShader* vs = new VertexShader();
	if (!vs->init(shader_byte_code, byte_code_size))
	{
		std::cout << "G.Engine FAIL 2\n";
		vs->release();
		return nullptr;
	}
	return vs;
}

PixelShader* GraphicsEngine::createPixelShader(const void* shader_byte_code, size_t byte_code_size)
{
	PixelShader* ps = new PixelShader();
	if (!ps->init(shader_byte_code, byte_code_size))
	{
		std::cout << "G.Engine FAIL 3\n";
		ps->release();
		return nullptr;
	}
	return ps;
}

bool GraphicsEngine::compileVertexShader(const wchar_t* file_name, const char* entry_point_name, void** shader_byte_code, size_t* byte_size_code)
{
	ID3DBlob* error_blob = nullptr;
	if (!SUCCEEDED(D3DCompileFromFile(file_name, nullptr, nullptr, entry_point_name, "vs_5_0", NULL, NULL, &m_blob, &error_blob)))
	{
		std::cout << "G.Engine FAIL 4\n";
		if (error_blob)error_blob->Release();
		return false;
	}
	*shader_byte_code = m_blob->GetBufferPointer();
	*byte_size_code = m_blob->GetBufferSize();

	return true;
}

bool GraphicsEngine::compilePixelShader(const wchar_t* file_name, const char* entry_point_name, void** shader_byte_code, size_t* byte_size_code)
{
	ID3DBlob* error_blob = nullptr;
	if (!SUCCEEDED(D3DCompileFromFile(file_name, nullptr, nullptr, entry_point_name, "ps_5_0", NULL, NULL, &m_blob, &error_blob)))
	{
		std::cout << "G.Engine FAIL 5\n";
		if (error_blob)error_blob->Release();
		return false;
	}
	*shader_byte_code = m_blob->GetBufferPointer();
	*byte_size_code = m_blob->GetBufferSize();

	return true;
}

void GraphicsEngine::releaseCompiledShader()
{
	if (m_blob)
	{
		m_blob->Release();
	}
}


GraphicsEngine * GraphicsEngine::get()
{
	return getInstance();
}