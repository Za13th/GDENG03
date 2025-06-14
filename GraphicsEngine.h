#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>

class SwapChain;
class DeviceContext;
class VertexBuffer;
class ConstantBuffer;
class IndexBuffer;
class VertexShader;
class PixelShader;


class GraphicsEngine
{
	public:
		static GraphicsEngine* getInstance();
		static void initialize();
		static void destroy();


		//Initialize 
		bool init();
		//Releases used resources
		bool release();


		SwapChain* createSwapChain();
		DeviceContext* getDeviceContext();
		VertexBuffer* createVertexBuffer();
		IndexBuffer* createIndexBuffer();
		ConstantBuffer* createConstantBuffer();
		VertexShader* createVertexShader(const void* shader_byte_code, size_t byte_code_size);
		PixelShader* createPixelShader(const void* shader_byte_code, size_t byte_code_size);

		bool compileVertexShader(const wchar_t* file_name, const char* entry_point_name, void** shader_byte_code, size_t* byte_size_code);
		bool compilePixelShader(const wchar_t* file_name, const char* entry_point_name, void** shader_byte_code, size_t* byte_size_code);

		void releaseCompiledShader();
		static GraphicsEngine* get();

	private:
		GraphicsEngine();
		~GraphicsEngine();
		GraphicsEngine(const GraphicsEngine&) {};
		GraphicsEngine& operator=(const GraphicsEngine&) {};
		static GraphicsEngine* sharedInstance;



		DeviceContext* m_imm_device_context;

		ID3D11Device* m_d3d_device;
		D3D_FEATURE_LEVEL m_feature_level;

		IDXGIDevice* m_dxgi_device;
		IDXGIAdapter* m_dxgi_adapter;
		IDXGIFactory* m_dxgi_factory;

		ID3DBlob* m_blob = nullptr;
		ID3DBlob* m_vsblob = nullptr;
		ID3DBlob* m_psblob = nullptr;
		ID3D11VertexShader* m_vs = nullptr;
		ID3D11PixelShader* m_ps = nullptr;

		friend class SwapChain;
		friend class VertexBuffer;
		friend class ConstantBuffer;
		friend class IndexBuffer;
		friend class VertexShader;
		friend class PixelShader;
};

