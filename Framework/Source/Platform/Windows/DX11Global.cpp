#include "pch.h"
#include "DX11Global.h"
#include <assert.h>
#include "WinExceptions.h"

namespace Framework {
	static DX11Global* instance;

	DX11Global::DX11Global()
	{
		// Only one instance of class allowed.
		assert(!instance);
		instance = this;

		UINT swapCreateFlags = 0u;
#ifdef DEBUG
		swapCreateFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		HRESULT hr;

		GFX_THROW_INFO(D3D11CreateDevice(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			swapCreateFlags,
			nullptr,
			0,
			D3D11_SDK_VERSION,
			&device,
			nullptr,
			&context));
	}

	DX11Global::~DX11Global()
	{
		instance = nullptr;
	}

	DX11Global& DX11Global::Get()
	{
		return *instance;
	}

}
