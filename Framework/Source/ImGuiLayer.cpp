#include "pch.h"
#include "ImGuiLayer.h"
#include "Window.h"
#include "Platform/Windows/GLFWWindow.h"
#include <imgui.h>
#pragma warning(push, 0)
#include <ImGui/imgui_impl_glfw.h>
#pragma warning(pop)

namespace Framework {
	void ImGuiLayer::OnAttach()
	{
		imGuiContext = ImGui::CreateContext();
		ImGui::SetCurrentContext(imGuiContext);

		ImGuiIO& io = ImGui::GetIO();

		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

		// Same style inside and outside windows.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		ImGui_ImplGlfw_InitForOther(static_cast<GLFWWindow*>(window.lock().get())->GetGLFWwindow(), true);
		GetGfx().ImGuiInit();
	}

	void ImGuiLayer::OnDetach() noexcept
	{
		GetGfx().ImGuiShutDown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext(imGuiContext);
	}

	void ImGuiLayer::OnEvent(Event& e)
	{
		const ImGuiIO& io = ImGui::GetIO();

		if(e.GetEventType() == EventType::mouse)
			e.handled |= io.WantCaptureMouse;
		else if(e.GetEventType() == EventType::keyboard)
			e.handled |=  io.WantCaptureKeyboard;
	}

	void ImGuiLayer::Begin()
	{
		ImGui::SetCurrentContext(imGuiContext);

		GetGfx().ImGuiNewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiLayer::End()
	{
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2((float)window.lock()->GetFramebufferSize().width, (float)window.lock()->GetFramebufferSize().height);

		ImGui::Render();
		GetGfx().ImGuiRenderDrawData(ImGui::GetDrawData());

		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
	}

	Framework::Graphics& ImGuiLayer::GetGfx() const
	{
		assert(!window.expired());
		return window.lock()->Gfx();
	}

}


