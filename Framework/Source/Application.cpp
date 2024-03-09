#include "pch.h"
#include "Application.h"
#include <cassert>

namespace Framework {
	static Application* appInstance;

	Application::Application()
	{
		//Only one instance of application allowed
		assert(!appInstance);
		appInstance = this;
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
}