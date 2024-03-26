#pragma once
#include "Graphics.h"
#include <d3d11.h>
#include <dxgi1_2.h>
#include <wrl.h>

namespace Framework {
	class DX11Graphics : public Graphics {
	public:
		DX11Graphics(Window* window, HWND hwnd);
		DX11Graphics(const DX11Graphics&) = delete;
		DX11Graphics& operator=(const DX11Graphics&) = delete;

		void StartFrame() override;
		void EndFrame() override;
		void ClearBuffer(float red, float green, float blue) override;

		void ImGuiInit() override;
		void ImGuiShutDown() override;
		void ImGuiNewFrame() override;
		void ImGuiRenderDrawData(ImDrawData* drawData) override;

	protected:
		void ResizeFramebuffer(int width, int height) override;

	private:
		Microsoft::WRL::ComPtr<IDXGISwapChain1> swap;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTarget;

		Window* window;
		HWND hwnd;
	};
}