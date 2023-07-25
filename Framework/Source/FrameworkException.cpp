#include "pch.h"
#include "FrameworkException.h"
#include <sstream>

namespace Framework
{
	FrameworkException::FrameworkException(int line, const std::string& file)
		: line(line), file(file)
	{}

	std::string FrameworkException::GetType() const { return "Framework Exception"; }

	std::string FrameworkException::GetMessage() const { return "Default framework exception"; }

	std::string FrameworkException::GetLocationString() const
	{
		std::stringstream ss;
		ss << "File: " << file << ", Line: " << line;
		return ss.str();
	}

	const char* FrameworkException::what() const
	{
		whatString = GetMessage();
		return whatString.c_str();
	}

}