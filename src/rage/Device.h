#pragma once
#include <d3d9.h>
#include "FunctorBase.h"
#include "../Utils.h"

namespace rage
{
	namespace grcDevice
	{
		struct grcResolveFlags 
		{
			grcResolveFlags()
				: Depth(1.0f)
				, BlurKernelSize(1.0f)
				, Color(0)
				, Stencil(0)
				, ColorExpBias(0)
				, ClearColor(false)
				, ClearDepthStencil(false)
				, BlurResult(false)
				, NeedResolve(true)
				, MipMap(true)
			{}

			float Depth;
			float BlurKernelSize;
			uint32_t Color;
			uint32_t Stencil;
			int ColorExpBias;
			bool ClearColor;
			bool ClearDepthStencil;
			bool BlurResult;
			bool NeedResolve;
			bool MipMap;
		};

		inline void RegisterDeviceLostCallbacks(Functor0 onLost, Functor0 onReset)
		{
			auto func = (void(__cdecl *)(Functor0, Functor0))Utils::ReadMemory(0x5730);
			func(onLost, onReset);
		}

		inline IDirect3DDevice9 *GetD3DDevice()
		{
			static IDirect3DDevice9 **device = nullptr;

			if(!device)
			{
				device = *(IDirect3DDevice9***)Utils::ReadMemory(0x6A03);
			}

			return *device;
		}

		inline uint32_t GetWidth()
		{
			static uint32_t *width = *(uint32_t**)Utils::ReadMemory(0x67AB);
			return *width;
		}

		inline uint32_t GetHeight()
		{
			static uint32_t *height = *(uint32_t**)Utils::ReadMemory(0x67A0);
			return *height;
		}
	}
}