#pragma once

#include <d3d11.h>

class DeviceContext;
class SwapChain
{
	public:
		SwapChain();
		//Initialize stuff
		bool init(HWND hwnd, UINT width, UINT height);

		bool present(bool vsync);

		//Releases used resources
		bool release();

		ID3D11RenderTargetView* getRenderTargetView() {return m_rtv;}
		ID3D11DepthStencilView* getDepthStencilView() { return m_depth_stencil_view; }

		~SwapChain();

	private:	
		IDXGISwapChain* m_swap_chain;
		ID3D11RenderTargetView* m_rtv;
		ID3D11DepthStencilView* m_depth_stencil_view;;

		friend class DeviceContext;

};

