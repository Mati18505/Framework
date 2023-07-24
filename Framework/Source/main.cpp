#include "pch.h"
#include "Application.h"

using namespace Framework;

int main(int argc, char* argv[])
{
	std::unique_ptr<Application> app = CreateApplication();
	app->Run();
}