#include "EventTest.h"
#include "MultipleWindowsTest.h"
#include "ConsoleTest.h"
#include "ImGuiTest.h"
#include "Event.h"
#include <queue>

using namespace Framework;

class TestsApplication : public Application {
public:
	TestsApplication()
		: Application()
	{
		testLayers.push(std::make_shared<MultipleWindowsTest>());
		testLayers.push(std::make_shared<ImGuiTest>());
		testLayers.push(std::make_shared<EventTest>());
		testLayers.push(std::make_shared<ConsoleTest>());

		GetLayerStack().PushLayer(testLayers.front());
		testLayers.pop();
	}
	void OnEvent(Event& e) override
	{
		GetLayerStack().OnEvent(e);

		Framework::EventDispatcher dispatcher(e);
		dispatcher.Dispatch<Framework::CustomEvent>([&](Framework::CustomEvent& e) {
			if (e.GetEventName() == "NextLayerEvent")
			{
				Next();
				return true;
			}
			return false;
		});

	}
private:
	std::queue<std::shared_ptr<Framework::Layer>> testLayers;

	void Next() {
		Application::Get().GetLayerStack().PopLayer();
		Application::Get().GetLayerStack().PushLayer(testLayers.front());
		testLayers.pop();
	}
};

std::unique_ptr<Framework::Application> Framework::CreateApplication()
{
	auto app = std::make_unique<TestsApplication>();
	return app;
}