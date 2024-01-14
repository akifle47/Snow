#pragma once
#include <cstdint>

namespace rage
{
	struct grcImage
	{
	public:
		uint16_t mWidth;
		uint16_t mHeight;
		int32_t mFormat;
		int32_t mType;
		uint16_t mStride;
		uint16_t mDepth;
		void *mPixelData;
		int32_t *field_14;
		grcImage *mNextLevel;
		grcImage *mNextSlice;
		uint32_t mRefCount;
		uint8_t gap24[8];
		int field_2C;
		float field_30[3];
		int32_t field_3C;
		float field_40[3];
		int field_4C;
	};

	static_assert(sizeof(grcImage) == 0x50);
}