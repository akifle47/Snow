#pragma once
#include <cstdint>
#include <d3d9.h>
#include "Texture.h"
#include "Base.h"

namespace rage
{
	class grcRenderTargetPC;
	struct grcRenderTargetDesc
	{
		grcRenderTargetDesc()
			: field_0(0)
			, mMultisampleCount(0)
			, field_8(1)
			, mLevels(1)
			, field_10(1)
			, field_11(1)
			, field_1C(1)
			, field_24(1)
			, field_26(1)
			, field_28(1)
			, field_12(0)
			, mDepthRT(0)
			, field_18(0)
			, field_20(0)
			, field_25(0)
			, field_27(0)
			, field_29(0)
			, field_2A(0)
			, mFormat(grctfUNKNOWN)
		{}

		char field_0;
		int mMultisampleCount;
		char field_8;
		int mLevels;
		char field_10;
		char field_11;
		char field_12;
		grcRenderTargetPC *mDepthRT;
		char field_18;
		int field_1C;
		int field_20;
		bool field_24;
		char field_25;
		char field_26;
		char field_27;
		char field_28;
		char field_29;
		char field_2A;
		alignas(4) grcTextureFormat mFormat;
	};

	//cant be bothered with figuring out the members of grcTexture
	class grcRenderTarget : public pgBase
	{
	public:
		uint8_t field_8;
		uint8_t field_9;
		int16_t field_A;
		int32_t field_C;
		int32_t field_10;
		uint32_t mType;
		int32_t *field_18;
		int16_t field_1C;
		int16_t field_1E;
		int32_t field_20;
		int16_t field_24;
		int16_t field_26;
		int32_t field_28;
		uint16_t field_2C;
		uint16_t field_2E;
	};

	static_assert(sizeof(grcRenderTargetDesc) == 0x30);

	class grcRenderTargetPC : public  grcRenderTarget
	{
	public:
		char *mName;
		IDirect3DTexture9 *mD3DTexture;
		IDirect3DSurface9 *mD3DSurface;
		uint16_t mWidth;
		uint16_t mHeight;
		grcTextureFormat mFormat;
		uint8_t mIndex;
		bool mBitsPerPixel;
		uint8_t mMultisampleCount;
		bool field_44;
		uint8_t gap45[2];
		uint8_t field_47;
		uint32_t mLevels;
		bool field_4C;
		bool field_4D;
		uint8_t field_4E;

		void Destroy(bool a2 = true)
		{
			auto func = (void (__thiscall *)(grcRenderTargetPC *, bool))(_vft[0]);
			func(this, a2);
		}
	};

	static_assert(sizeof(grcRenderTargetPC) == 0x50);
}