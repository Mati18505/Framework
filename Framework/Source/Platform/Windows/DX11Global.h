#pragma once
#include "DxgiInfoManager.h"
#include <d3d11.h>

namespace Framework {
	class DX11Global {
	public:
		DX11Global();
		~DX11Global();
		DX11Global(const DX11Global&) = delete;
		DX11Global& operator=(const DX11Global&) = delete;

		static DX11Global& Get();

#ifdef DEBUG
		inline static DxgiInfoManager infoManager;
#endif

		static inline Microsoft::WRL::ComPtr<ID3D11Device> device;
		static inline Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;
	};
}