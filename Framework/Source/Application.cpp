#include "pch.h"
#include "Application.h"
#include <cassert>
#include <Logger.h>
#include "Platform/Platform.h"

namespace Framework {
	static Application* appInstance;

	Application::Application()
	{
		//Only one instance of application allowed
		assert(!appInstance);
		appInstance = this;

		Platform::Init();
		Logger::Init();
	}

	Application::~Application()
	{
		layerStack.Flush();
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