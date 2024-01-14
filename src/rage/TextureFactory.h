#pragma once
#include "RenderTarget.h"
#include "Texture.h"
#include "Image.h"
#include "../Utils.h"

namespace rage
{
	class grcTextureFactoryPC;
	class grcTextureFactory
	{
	protected:
		uint32_t *_vft;

	public:
		bool field_4;
		char _gap5[3];

		static grcTextureFactoryPC *GetInstance()
		{
			static grcTextureFactoryPC *instance = nullptr;
			if(!instance)
			{
				instance = **(rage::grcTextureFactoryPC***)Utils::ReadMemory(0x1B604C);
			}

			return instance;
		}
	};

	class grcTextureFactoryPC : public grcTextureFactory
	{
	public:
		IDirect3DSurface9 *mPrevRenderTargets[3];
		int32_t field_14;
		int32_t field_18;
		int32_t field_1C;
		int32_t field_20;
		int32_t field_24;
		IDirect3DSurface9 *mD3D9Surfaces[4];
		uint8_t gap38[4];
		int32_t field_3C;
		int32_t field_40;
		int32_t field_44;
		IDirect3DSurface9 *field_48;
		IDirect3DSurface9 *mDepthStencilSurface;
		IDirect3DSurface9 *field_50;
		grcRenderTargetPC *field_54;
		int32_t field_58;
		int32_t field_5C;
		int32_t field_60;
		int32_t field_64;
		int32_t field_68;
		int32_t field_6C;
		int32_t field_70;
	
		//virtuals
		grcTexturePC *Create(grcImage *image, void *arg2)
		{
			auto func = (grcTexturePC *(__stdcall *)(grcImage *, void *))(_vft[2]);
			return func(image, arg2);
		}

		grcRenderTargetPC *__stdcall CreateRenderTarget(const char *name, int32_t a2, uint32_t width, uint32_t height, uint32_t bitsPerPixel, grcRenderTargetDesc *desc)
		{
			auto func = (grcRenderTargetPC *(__stdcall *)(const char *, int32_t, uint32_t, uint32_t, uint32_t, grcRenderTargetDesc*))(_vft[14]);
			return func(name, a2, width, height, bitsPerPixel, desc);
		}

		void LockRenderTarget(uint32_t index, grcRenderTargetPC *color, grcRenderTargetPC *depth, uint32_t a5 = 0, bool a6 = 1, uint32_t mip = 0)
		{
			auto func = (void (__thiscall *)(grcTextureFactory *, uint32_t, grcRenderTargetPC*, grcRenderTargetPC*, uint32_t, bool, uint32_t))(_vft[15]);
			func(this, index, color, depth, a5, a6, mip);
		}

		void UnlockRenderTarget(uint32_t index, grcDevice::grcResolveFlags *resolveFlags, int32_t unused = -1)
		{
			auto func = (void (__thiscall *)(grcTextureFactory *, uint32_t, grcDevice::grcResolveFlags *, int32_t))(_vft[16]);
			func(this, index, resolveFlags, unused);
		}
	};

	static_assert(sizeof(grcTextureFactoryPC) == 0x74);
}