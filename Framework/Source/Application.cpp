#include "pch.h"
#include "Application.h"
#include <cassert>
#include "Platform/PlatformDetector.h"
#ifdef OS_WIN
#include "Platform/Windows/WindowsOSInitializer.h"
#endif
#include <Logger.h>

namespace Framework {
	static Application* appInstance;

	Application::Application()
	{
		//Only one instance of application allowed
		assert(!appInstance);
		appInstance = this;

#ifdef OS_WIN
		WindowsOSInitializer::Init();
#endif
		Logger::Init();
	}

	Application::~Application()
	{
		appInstance = nullptr;
	}

	void Application::Run()
	{
		while (running) 
		{
			layerStack.OnUpdate();
			layerStack.OnRender();
			layerStack.Update();
		}
	}
	Application& Application::Get() { return *appInstance; }

	void Application::OnEvent(Event& e)
	{
		GetLayerStack().OnEvent(e);
	}
}