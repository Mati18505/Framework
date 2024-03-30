#include "pch.h"
#include "Container.h"

namespace Framework::ioc
{
	Container& Container::Get()
	{
		static Container c;
		return c;
	}
}
