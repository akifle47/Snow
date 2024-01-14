#pragma once
#include <cstdint>
#include "Base.h"

namespace rage
{
	enum grcTextureFormat : uint8_t
	{
		grctfUNKNOWN = 0x0,
		grctfR5G6B5 = 0x1,
		grctfA8R8G8B8 = 0x2,
		grctfR16F = 0x3,
		grctfR32F = 0x4,
		grctfA2R10G10B10 = 0x5,
		grctfA16B16G16R16F = 0x6,
		grctfG16R16 = 0x7,
		grctfG16R16F = 0x8,
		grctfA32B32G32R32F = 0x9,
		grctfA16B16G16R16F_ = 0xA,
		grctfA16B16G16R16 = 0xB,
		grctfL8 = 0xC,
		grctfA1R5G5B5 = 0xD,
		grctfD24S8 = 0xE,
		grctfD32F_LOCKABLE = 0xF,
		grctfX8R8G8B8 = 0x10,
		grctfR16F_ = 0x11,
		grctfG32R32F = 0x12,
	};

	//cant be bothered with figuring out the members of grcTexture
	class grcTexturePC : pgBase
	{
	public:
		uint8_t field_8;
		uint8_t mDepth;
		uint16_t mRefCount;
		int32_t field_C;
		int32_t field_10;
		char *mName;
		IDirect3DTexture9 *mD3DTexture;
		uint16_t mWidth;
		uint16_t mHeight;
		int32_t mFormat;
		uint16_t mStride;
		uint8_t mTextureType;
		uint8_t mMipCount;
		D3DVECTOR field_28;
		D3DVECTOR field_34;
		grcTexturePC *mPrevious;
		grcTexturePC *mNext;
		void *mPixelData;
		uint8_t field_4C;
		uint8_t field_4D;
		uint8_t field_4E;
		uint8_t field_4F;

		bool Init()
		{
			auto func = (grcTexturePC *(__thiscall *)(grcTexturePC *))(_vft[4]);
			return func(this);
		}
	};

	static_assert(sizeof(grcTexturePC) == 0x50);
}