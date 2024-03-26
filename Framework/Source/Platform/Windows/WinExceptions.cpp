#include "pch.h"
#include "WinExceptions.h"

namespace Framework {
	std::string WinException::TranslateErrorCode(HRESULT hr)
	{
		char* pMsgBuf = nullptr;
		// Windows will allocate memory for err string and make our pointer point to it.
		const DWORD nMsgLen = FormatMessageA(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			reinterpret_cast<LPSTR>(&pMsgBuf), 0, nullptr
		);

		std::string errorString;

		// 0 string length returned indicates a failure.
		if (nMsgLen == 0)
			errorString = "Unidentified error code";
		else
		{
			errorString = pMsgBuf;
			LocalFree(pMsgBuf);
			if (errorString.ends_with("\r\n")) {
				errorString.resize(errorString.size() - 2);
			}
		}

		return errorString;
	}

	namespace Gfx {
		// Graphics exception stuff.
		HrException::HrException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs)
			:
			WinException(line, file),
			hr(hr)
		{
			// Join all info messages with newlines into single string.
			for (const auto& m : infoMsgs)
			{
				info += m;
				info.push_back('\n');
			}
			// Remove final newline if exists.
			if (!info.empty())
			{
				info.pop_back();
			}
		}

		std::string HrException::GetMessage() const
		{
			std::ostringstream oss;
			oss << std::endl
				<< "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
				<< std::dec << " (" << (unsigned long)GetErrorCode() << ")" << std::endl
				<< "[Description] " << GetErrorDescription() << std::endl;
			if (!info.empty())
			{
				oss << "[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;
			}
			return oss.str();
		}

		std::string HrException::GetType() const
		{
			return "Graphics Exception";
		}

		HRESULT HrException::GetErrorCode() const
		{
			return hr;
		}

		std::string HrException::GetErrorDescription() const
		{
			return TranslateErrorCode(hr);
		}

		std::string HrException::GetErrorInfo() const
		{
			return info;
		}

		InfoException::InfoException(int line, const char* file, std::vector<std::string> infoMsgs)
			:
			WinException(line, file)
		{
			for (const auto& m : infoMsgs)
			{
				info += m;
				info.push_back('\n');
			}

			if (!info.empty())
			{
				info.pop_back();
			}
		}

		std::string InfoException::GetType() const
		{
			return "Graphics Info Exception";
		}

		std::string InfoException::GetMessage() const
		{
			std::ostringstream oss;
			oss << std::endl << "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;
			return oss.str();
		}

		std::string InfoException::GetErrorInfo() const
		{
			return info;
		}

		std::string DeviceRemovedException::GetType() const
		{
			return "Graphics Exception [Device Removed] (DXGI_ERROR_DEVICE_REMOVED)";
		}
	}
}