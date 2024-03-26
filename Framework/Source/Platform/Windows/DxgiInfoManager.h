#pragma once
#include "FWWin.h"
#include <dxgidebug.h>
#include <wrl.h>

namespace Framework
{
	class DxgiInfoManager
	{
	public:
		DxgiInfoManager();
		~DxgiInfoManager() = default;
		DxgiInfoManager(const DxgiInfoManager&) = delete;
		DxgiInfoManager& operator=(const DxgiInfoManager&) = delete;

		void Set() noexcept;
		std::vector<std::string> GetMessages() const;
	private:
		UINT64 next = 0u;
		Microsoft::WRL::ComPtr<IDXGIInfoQueue> pDxgiInfoQueue;
	};
}