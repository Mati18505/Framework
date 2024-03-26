#pragma once
#include "FrameworkException.h"
#include "DX11Global.h"

#define GFX_G ::Framework::Gfx

#define GFX_EXCEPT_NOINFO(hr) GFX_G::HrException( __LINE__,__FILE__,(hr) )
#define GFX_THROW_NOINFO(hrcall) if( FAILED( hr = (hrcall) ) ) throw GFX_G::HrException( __LINE__,__FILE__,hr )

#ifdef DEBUG
#define GFX_EXCEPT(hr) GFX_G::HrException( __LINE__,__FILE__,(hr),DX11Global::infoManager.GetMessages() )
#define GFX_THROW_INFO(hrcall) DX11Global::infoManager.Set(); if( FAILED( hr = (hrcall) ) ) throw GFX_EXCEPT(hr)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) GFX_G::DeviceRemovedException( __LINE__,__FILE__,(hr),DX11Global::infoManager.GetMessages() )
#define GFX_THROW_INFO_ONLY(call) DX11Global::infoManager.Set(); (call); {auto v = DX11Global::infoManager.GetMessages(); if(!v.empty()) {throw GFX_G::InfoException( __LINE__,__FILE__,v);}}
#else
#define GFX_EXCEPT(hr) GFX_EXCEPT_NOINFO(hr)
#define GFX_THROW_INFO(hrcall) GFX_THROW_NOINFO(hrcall)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) GFX_G::DeviceRemovedException( __LINE__,__FILE__,(hr) )
#define GFX_THROW_INFO_ONLY(call) (call)
#endif

namespace Framework {
	class WinException : public FrameworkException
	{
		using FrameworkException::FrameworkException;
	public:
		static std::string TranslateErrorCode(HRESULT hr);
	};

	namespace Gfx {
		class HrException : public WinException
		{
		public:
			HrException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs = {});

			std::string GetType() const override;
			std::string GetMessage() const override;

			std::string GetErrorDescription() const;
			std::string GetErrorInfo() const;

			HRESULT GetErrorCode() const;
		private:
			HRESULT hr;
			std::string info;
		};

		class InfoException : public WinException
		{
		public:
			InfoException(int line, const char* file, std::vector<std::string> infoMsgs);

			std::string GetType() const override;
			std::string GetMessage() const override;

			std::string GetErrorInfo() const;
		private:
			std::string info;
		};

		class DeviceRemovedException : public HrException
		{
			using HrException::HrException;
		public:
			std::string GetType() const override;
		};
	}
}