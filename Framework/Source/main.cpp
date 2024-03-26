#include "pch.h"
#include "Application.h"
#include "FrameworkException.h"

using namespace Framework;

int main(int argc, char* argv[])
{
	try
	{
		std::unique_ptr<Application> app = CreateApplication();
		app->Run();
	}
	catch (const FrameworkException& e)
	{
		std::cout << "Unhandled Exception:" << std::endl << "Type: " << e.GetType() << ", " << std::endl << e.GetLocationString() << std::endl << "Message: " << e.what() << std::endl;
	}
	catch(const std::exception& e)
	{
		std::cout << "Unhandled STL Exception: " << ", Description: " << e.what() << std::endl;
	}
	return 0;
}