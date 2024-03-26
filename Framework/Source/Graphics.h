#pragma once
struct ImDrawData;

namespace Framework {
	class Graphics {
		friend class Window;
	public:
		virtual ~Graphics() = default;
		
		virtual void StartFrame() = 0;
		virtual void EndFrame() = 0;
		virtual void ClearBuffer(float red, float green, float blue) = 0;

		virtual void ImGuiInit() = 0;
		virtual void ImGuiShutDown() = 0;
		virtual void ImGuiNewFrame() = 0;
		virtual void ImGuiRenderDrawData(ImDrawData* drawData) = 0;

	protected:
		virtual void ResizeFramebuffer(int width, int height) = 0;
	};

}