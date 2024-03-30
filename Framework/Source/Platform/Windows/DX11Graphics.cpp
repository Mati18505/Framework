#include "pch.h"
#include "DX11Graphics.h"
#include "DX11Global.h"
#include "WinExceptions.h"
#include "DxgiInfoManager.h"
#include <d3dcompiler.h>
#pragma warning(push, 0)
#include <ImGui/imgui_impl_dx11.h>
#pragma warning(pop)

namespace Framework {

	DX11Graphics::DX11Graphics(IoCParams params, HWND hwnd)
		: window(params.window), hwnd(hwnd)
	{
		DXGI_SAMPLE_DESC sampleDesc;
		sampleDesc.Count = 1;
		sampleDesc.Quality = 0;

		DXGI_SWAP_CHAIN_DESC1 scd = {};
		scd.Width = 0;
		scd.Height = 0;
		scd.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		scd.Stereo = FALSE;
		scd.SampleDesc = sampleDesc;
		scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		scd.BufferCount = 1;
		scd.Scaling = DXGI_SCALING_STRETCH;
		scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		scd.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
		scd.Flags = 0;

		HRESULT hr;

		auto device = DX11Global::device;

		Microsoft::WRL::ComPtr<IDXGIDevice2> pDXGIDevice;
		GFX_THROW_INFO(device->QueryInterface(__uuidof(IDXGIDevice2), &pDXGIDevice));

		Microsoft::WRL::ComPtr<IDXGIAdapter> pDXGIAdapter;
		GFX_THROW_INFO(pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), &pDXGIAdapter));

		Microsoft::WRL::ComPtr<IDXGIFactory2> pIDXGIFactory;
		GFX_THROW_INFO(pDXGIAdapter->GetParent(__uuidof(IDXGIFactory2), &pIDXGIFactory));

		GFX_THROW_INFO(pIDXGIFactory->CreateSwapChainForHwnd(
			device.Get(),
			hwnd,
			&scd,
			nullptr,
			nullptr,
			&swap
		));

		Microsoft::WRL::ComPtr<ID3D11Resource> backBuffer;
		GFX_THROW_INFO(swap->GetBuffer(0, __uuidof(ID3D11Resource), &backBuffer));
		GFX_THROW_INFO(device->CreateRenderTargetView(backBuffer.Get(), nullptr, renderTarget.GetAddressOf()));
	}

	void DX11Graphics::StartFrame()
	{
		DX11Global::context->OMSetRenderTargets(1, renderTarget.GetAddressOf(), nullptr);
	}

	void DX11Graphics::EndFrame()
	{
		HRESULT hr;

#ifdef DEBUG
		DX11Global::infoManager.Set();
#endif

		if(FAILED(hr = swap->Present(1u, 0u)))
		{
			if (hr == DXGI_ERROR_DEVICE_REMOVED)
				throw GFX_DEVICE_REMOVED_EXCEPT(DX11Global::device->GetDeviceRemovedReason());
			else
				throw GFX_EXCEPT(hr);
		}
	}

	void DX11Graphics::ClearBuffer(float red, float green, float blue)
	{
		const float color[] = { red,green,blue };
		DX11Global::context->ClearRenderTargetView(renderTarget.Get(), color);
	}

	void DX11Graphics::ImGuiInit()
	{
		ImGui_ImplDX11_Init(DX11Global::device.Get(), DX11Global::context.Get());
	}

	void DX11Graphics::ImGuiShutDown()
	{
		ImGui_ImplDX11_Shutdown();
	}

	void DX11Graphics::ImGuiNewFrame()
	{
		ImGui_ImplDX11_NewFrame();
	}

	void DX11Graphics::ImGuiRenderDrawData(ImDrawData* drawData)
	{
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}

	void DX11Graphics::ResizeFramebuffer(int width, int height)
	{
		if (swap)
		{
			DX11Global::context->OMSetRenderTargets(0, nullptr, nullptr);

			renderTarget->Release();

			HRESULT hr;
			// Preserve the existing buffer count and format.
			// Automatically choose the width and height to match the client rect for HWNDs.
			GFX_THROW_INFO(swap->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0));

			// Get buffer and create a render-target-view.
			Microsoft::WRL::ComPtr<ID3D11Texture2D> buffer;
			GFX_THROW_INFO(swap->GetBuffer(0, __uuidof(ID3D11Texture2D), &buffer));

			GFX_THROW_INFO(DX11Global::device->CreateRenderTargetView(buffer.Get(), nullptr, &renderTarget));

			DX11Global::context->OMSetRenderTargets(1, renderTarget.GetAddressOf(), nullptr);

			// Set up the viewport.
			D3D11_VIEWPORT vp;
			vp.Width = (float)width;
			vp.Height = (float)height;
			vp.MinDepth = 0.0f;
			vp.MaxDepth = 1.0f;
			vp.TopLeftX = 0;
			vp.TopLeftY = 0;
			DX11Global::context->RSSetViewports(1, &vp);
		}
	}
}