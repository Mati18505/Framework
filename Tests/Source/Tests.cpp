#include "EventTest.h"
#include "MultipleWindowsTest.h"
#include "ConsoleTest.h"
#include "Event.h"
#include <queue>

using namespace Framework;

class TestLayer : public Framework::Layer {
public:
	std::queue<std::shared_ptr<Framework::Layer>> testLayers;

	explicit TestLayer(const std::queue<std::shared_ptr<Framework::Layer>>& testLayers)
		: testLayers(testLayers)
	{}

	void OnAttach() override {
		Application::Get().GetLayerStack().PushLayer(testLayers.front());
		testLayers.pop();
	}
	void OnUpdate() override {}
	void OnRender() override {}
	void OnEvent(Framework::Event& e) override {
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
	void OnDetach() noexcept override {}

	void Next() {
		Application::Get().GetLayerStack().PopLayer();
		Application::Get().GetLayerStack().PushLayer(testLayers.front());
		testLayers.pop();
	}
};

std::unique_ptr<Framework::Application> Framework::CreateApplication()
{
	auto app = std::make_unique<Application>();

	std::queue<std::shared_ptr<Framework::Layer>> layers;
	layers.push(std::make_shared<EventTest>());
	layers.push(std::make_shared<ConsoleTest>());
	layers.push(std::make_shared<MultipleWindowsTest>());

	std::shared_ptr<Layer> testLayer = std::make_shared<TestLayer>(layers);
	app->GetLayerStack().PushLayer(testLayer);

	return app;
}