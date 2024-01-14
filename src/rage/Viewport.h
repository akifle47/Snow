#pragma once
#include "Vector.h"

namespace rage
{
	struct grcViewportWindow
	{
		float X;
		float Y;
		float Width;
		float Height;
		float MinZ;
		float MaxZ;
	};

	class grcViewport
	{
	public:
		float mWorldMatrix44[4][4];
		float mCameraMatrix[4][4];
		float mWorldMatrix[4][4];
		float mWorldViewMatrix[4][4];
		float mWorldViewProjMatrix[4][4];
		float mViewInverseMatrix[4][4];
		float mViewMatrix[4][4];
		float mProjectionMatrix[4][4];
		float mMatrix_200[4][4];//FrustumLRTB?
		float mMatrix_240[4][4];//FrustumNFNF?
		grcViewportWindow mGrcViewportWindow1;
		grcViewportWindow mGrcViewportWindow2;//UnclippedWindow?
		int mWidth;
		int mHeight;
		float mFov;
		float mAspect;
		float mNearClip;
		float mFarClip;
		float field_2C8;
		float field_2CC;
		float mScaleX;
		float mScaleY;
		float field_2D8;
		float field_2DC;
		Vector4 field_2E0;
		bool mIsPerspective;
		char gap_2f1[3];
		int field_2F4;
		int field_2F8;
		int field_2FC;
		Vector4 mFrustumClipPlanes[6];
		int field_360;
		int field_364;
		int field_368;
		int field_36C;
		int field_370;
		int field_374;
		int field_378;
		int field_37C;
		int field_380;
		int field_384;
		int field_388;
		int field_38C;
		int field_390;
		int field_394;
		int field_398;
		int field_39C;
		int field_3A0;
		int field_3A4;
		int field_3A8;
		int field_3AC;
		int field_3B0;
		int field_3B4;
		int field_3B8;
		int field_3BC;
		int field_3C0;
		int field_3C4;
		int field_3C8;
		int field_3CC;
		int field_3D0;
		int field_3D4;
		int field_3D8;
		int field_3DC;
		bool mInvertZInProjectionMatrix;
		char field_3E1[3];
		int field_3E4;
		int field_3E8;
		int field_3EC;

		static grcViewport *GetCurrent()
		{
			static rage::grcViewport **viewport = *(rage::grcViewport ***)Utils::ReadMemory(0x13F7B);
			return *viewport;
		}
	};

	static_assert(sizeof(grcViewport) == 0x3F0);
}