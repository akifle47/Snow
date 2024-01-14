#pragma once
#include "Vector.h"
#include <cstdint>

namespace rage
{
	enum eLightType
	{
		LT_POINT = 0x0,
		LT_DIR = 0x1,
		LT_SPOT = 0x2,
		LT_AO = 0x3,
		LT_CLAMPED = 0x4,
	};

	class CLightSource
	{
	public:
		Vector3 mDirection;
		int32_t field_C;
		Vector3 mTangent;
		int32_t field_1C;
		Vector3 mPosition;
		int32_t field_2C;
		Vector4 mColor;
		float mIntensity;
		eLightType mType;
		int32_t mFlags;
		int32_t mTxdId;
		int32_t field_50;
		float mRadius;
		float mInnerConeAngle;
		float mOuterConeAngle;
		int32_t field_60;
		int32_t field_64;
		int32_t field_68;
		int32_t field_6C;
		float mVolumeSize;
		float mVolumeScale;
		uint8_t gap78[8];
	};

	static_assert(sizeof(CLightSource) == 0x80);
}