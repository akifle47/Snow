#define _CRT_SECURE_NO_WARNINGS
#define LOG_DEPTH 1

#include "SnowRenderer.h"
#include "EmbeddedShaders.h"

namespace SnowRenderer
{
	bool mCfgSnowOnEveryWeather = false;
	float mCfgVolumetricIntensityMultiplier = 1.0f;

	float mVolumeIntensity = 1.0f;

	IDirect3DVertexBuffer9 *mQuadVertexBuffer;
	IDirect3DVertexDeclaration9 *mQuadVertexDecl;

	IDirect3DVertexShader9 *mSnowVS;
	IDirect3DPixelShader9 *mSnowPS;
	IDirect3DTexture9 *mSnowTexture;

	IDirect3DVertexShader9 *mBlitVS;
	IDirect3DPixelShader9 *mBlitPS;

	uint32_t *mFrameIndex = nullptr;
	uint32_t mHeightRenderPhase = 0;
	rage::grcRenderTargetPC *mDiffuseRT;
	rage::grcRenderTargetPC *mNormalRT;
	rage::grcRenderTargetPC *mSpecularAoRT;
	rage::grcRenderTargetPC *mStencilRT;
	rage::grcRenderTargetPC *mDepthRT;

	rage::grcRenderTargetPC *mNormalRtCopy;

	void __fastcall OnDeviceLost();
	void __fastcall OnDeviceReset();

	void Init();

	void BuildRenderList()
	{
		Init();

		auto mBeforeLightingCB = new rage::T_CB_Generic_NoArgs<void(__cdecl *)(void)>(OnBeforeLighting);
		mBeforeLightingCB->Init();
	}

	void Init()
	{
		static bool initialized = false;
		if(initialized)
			return;
		
		std::ifstream cfgFile("Snow.cfg");
		if(cfgFile.is_open())
		{
			std::string currLine;
			while(std::getline(cfgFile, currLine))
			{
				size_t index;
				index = currLine.rfind("VolumetricIntensityMultiplier");
				if(index != std::string::npos)
				{
					index += sizeof("VolumetricIntensityMultiplier");

					while(index < currLine.size())
					{
						if(currLine[index] != ' ' && currLine[index] != '=')
						{
							break;
						}
						
						index++;
					}

					mCfgVolumetricIntensityMultiplier = std::stof(currLine.substr(index));
				}

				if(currLine.rfind("SnowOnEveryWeather") != std::string::npos)
				{
					if(currLine.rfind("true") != std::string::npos)
					{
						mCfgSnowOnEveryWeather = true;
					}
				}
			}
		}

		//movss xmm0, dword ptr [esi + eax * 1 + 0xC] or xmm0 = Lights::mRenderLights[i].field_C
		uint8_t buffer[] = {0x44, 0x06, 0xC, 0x90, 0x90};
		Utils::WriteMemory(0x630B00, buffer, 5);

		mFrameIndex = *(uint32_t**)Utils::ReadMemory(0x455DA7);
		mFrameIndex = (uint32_t*)((uint8_t*)mFrameIndex + 0xFC4);
		//todo: breaks on device loss fix it!!!
		mHeightRenderPhase = (uint32_t)**(uint32_t***)Utils::ReadMemory(0x888961);

		rage::Functor0 onLostCB = rage::Functor0(NULL, OnDeviceLost, NULL, 0);
		rage::Functor0 onResetCB = rage::Functor0(NULL, OnDeviceReset, NULL, 0);

		rage::grcDevice::RegisterDeviceLostCallbacks(onLostCB, onResetCB);

		rage::grcDevice::GetD3DDevice()->CreateVertexShader((DWORD *)gShaderDataBlitVS, &mBlitVS);
		rage::grcDevice::GetD3DDevice()->CreatePixelShader((DWORD *)gShaderDataBlitPS, &mBlitPS);

		rage::grcDevice::GetD3DDevice()->CreateVertexShader((DWORD *)gShaderDataSnowVS, &mSnowVS);
		rage::grcDevice::GetD3DDevice()->CreatePixelShader((DWORD *)gShaderDataSnowPS, &mSnowPS);

		OnDeviceReset();

		initialized = true;
	}

	void __fastcall OnDeviceLost()
	{
		mDiffuseRT = nullptr;
		mNormalRT = nullptr;
		mSpecularAoRT = nullptr;
		mStencilRT = nullptr;
		mDepthRT = nullptr;

		if(mQuadVertexBuffer)
		{
			mQuadVertexBuffer->Release();
			mQuadVertexBuffer = nullptr;
		}

		if(mQuadVertexDecl)
		{
			mQuadVertexDecl->Release();
			mQuadVertexBuffer = nullptr;
		}

		if(mSnowTexture)
		{
			mSnowTexture->Release();
			mSnowTexture = nullptr;
		}

		if(mNormalRtCopy)
		{ 
			mNormalRtCopy->Destroy();
		}
	}

	void __fastcall OnDeviceReset()
	{
		rage::grcRenderTargetPC **gbufferRTs = *(rage::grcRenderTargetPC ***)Utils::ReadMemory(0x695D6E);
		mDiffuseRT = gbufferRTs[0];
		mNormalRT = gbufferRTs[1];
		mSpecularAoRT = gbufferRTs[2];
		mDepthRT = gbufferRTs[3];
		mStencilRT = **(rage::grcRenderTargetPC ***)Utils::ReadMemory(0x695E5A);

		uint32_t width = rage::grcDevice::GetWidth();
		uint32_t height = rage::grcDevice::GetHeight();
		
		rage::grcRenderTargetDesc desc {};
		desc.mMultisampleCount = 0;
		desc.field_0 = 1;
		desc.field_12 = 1;
		desc.mDepthRT = nullptr;
		desc.field_8 = 1;
		desc.field_10 = 1;
		desc.field_11 = 1;
		desc.field_24 = false;
		desc.mFormat = rage::grctfA2R10G10B10;

		mNormalRtCopy = rage::grcTextureFactory::GetInstance()->CreateRenderTarget("_DEFERRED_GBUFFER_1_COPY", 3, width, height, 32, &desc);

		D3DXCreateTextureFromFile(rage::grcDevice::GetD3DDevice(), L"Snow.dds", &mSnowTexture);

		D3DVERTEXELEMENT9 vertexDeclElements[] =
		{
			{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
			{0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
			D3DDECL_END()
		};

		rage::grcDevice::GetD3DDevice()->CreateVertexDeclaration(vertexDeclElements, &mQuadVertexDecl);

		rage::grcDevice::GetD3DDevice()->CreateVertexBuffer(6 * sizeof(VertexFormat), 0, 0, D3DPOOL_DEFAULT, &mQuadVertexBuffer, NULL);

		VertexFormat *vertexData;
		mQuadVertexBuffer->Lock(0, 0, (void **)&vertexData, 0);

		vertexData[0] = {-1.0f, -1.0f, 0.0f, 0.0f, 1.0f};
		vertexData[1] = {-1.0f, 1.0f, 0.0f, 0.0f, 0.0f};
		vertexData[2] = {1.0f, -1.0f, 0.0f, 1.0f, 1.0f};
		vertexData[3] = {-1.0f, 1.0f, 0.0f, 0.0f, 0.0f};
		vertexData[4] = {1.0f, 1.0f, 0.0f, 1.0f, 0.0f};
		vertexData[5] = {1.0f, -1.0f, 0.0f, 1.0f, 1.0f};

		mQuadVertexBuffer->Unlock();

#ifdef  _DEBUG

		HRESULT hr;
		LPD3DXBUFFER vertexShaderData = nullptr;
		LPD3DXBUFFER pixelShaderData = nullptr;
		LPD3DXBUFFER errorBuffer = nullptr;

		hr = D3DXCompileShaderFromFile(L"shaders/Snow.vs", NULL, NULL, "main", "vs_3_0", 0, &vertexShaderData, &errorBuffer, 0);
		if(FAILED(hr))
		{
			MessageBoxA(0, (char *)errorBuffer->GetBufferPointer(), "VS ERROR", MB_OK);
			return;
		}

		mSnowVS->Release();
		rage::grcDevice::GetD3DDevice()->CreateVertexShader((DWORD *)vertexShaderData->GetBufferPointer(), &mSnowVS);

		hr = D3DXCompileShaderFromFile(L"shaders/Snow.ps", NULL, NULL, "main", "ps_3_0", 0, &pixelShaderData, &errorBuffer, 0);
		if(FAILED(hr))
		{
			MessageBoxA(0, (char *)errorBuffer->GetBufferPointer(), "PS ERROR", MB_OK);
			return;
		}

		mSnowPS->Release();
		rage::grcDevice::GetD3DDevice()->CreatePixelShader((DWORD *)pixelShaderData->GetBufferPointer(), &mSnowPS);

#endif //_DEBUG
	}

	bool HasSnow(CWeather::eWeatherType type)
	{
		if(mCfgSnowOnEveryWeather)
		{
			return true;
		}

		switch(type)
		{
			case CWeather::RAIN:
			case CWeather::DRIZZLE:
				return true;
			default:
				return false;
		}
	}

	void OnAfterCopyLight(rage::CLightSource *light)
	{
		//CLightSource doesnt have a member to control the volume intensity so
		//i abuse type casting to use field_C for it as im p sure its just padding anyway

		if(light->mFlags & 8 /*volumetric*/)
		{
			*(float*)&light->field_C = 1.0f;
		}
		else if(light->mType == rage::LT_SPOT && !(light->mFlags & 0x300)/*vehicles and traffic lights*/)
		{
			if(mVolumeIntensity > 0.0f && light->mRadius < 50.0f)
			{
				light->mVolumeSize = 1.0f;
				light->mVolumeScale = 0.5f;
				//light->field_64 = -1; //not really sure what this is but setting it to -1 makes the light not cast shadows
				light->mFlags |= 8;
				*(float*)&light->field_C = mVolumeIntensity;
			}
		}
	}

	void OnBeforeLighting()
	{
		IDirect3DDevice9 *device = rage::grcDevice::GetD3DDevice();

		mVolumeIntensity = 1.0f;

		static CWeather::eWeatherType currWeather;

		CWeather::eWeatherType prevWeather = currWeather;
		currWeather = CWeather::GetCurrentWeather();
		CWeather::eWeatherType nextWeather = CWeather::GetNextWeather();

		if(HasSnow(prevWeather) || HasSnow(currWeather) || HasSnow(nextWeather))
		{
			DWORD prevMinFilters[6];
			DWORD prevMagFilters[6];
			for(uint32_t i = 0; i < 6; i++)
			{
				device->GetSamplerState(i, D3DSAMP_MINFILTER, &prevMinFilters[i]);
				device->GetSamplerState(i, D3DSAMP_MAGFILTER, &prevMagFilters[i]);

				device->SetSamplerState(i, D3DSAMP_MINFILTER, D3DTEXF_POINT);
				device->SetSamplerState(i, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
			}

			rage::grcViewport *currGrcViewport = rage::grcViewport::GetCurrent();

			rage::Vector4 pixelOffset;
			pixelOffset.x = (-1.0f / mNormalRT->mWidth);
			pixelOffset.y = (1.0f / mNormalRT->mHeight);
			device->SetVertexShaderConstantF(5, &pixelOffset.x, 1);

			//copy normal gbuffer
			{
				rage::grcTextureFactoryPC::GetInstance()->LockRenderTarget(0, mNormalRtCopy, nullptr);
				device->SetVertexShader(mBlitVS);
				device->SetPixelShader(mBlitPS);

				device->SetVertexDeclaration(mQuadVertexDecl);
				device->SetStreamSource(0, mQuadVertexBuffer, 0, sizeof(VertexFormat));

				device->SetTexture(0, mNormalRT->mD3DTexture);

				device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);

				rage::grcDevice::grcResolveFlags resolveFlags {};
				rage::grcTextureFactoryPC::GetInstance()->UnlockRenderTarget(0, &resolveFlags);
			}

			//render snow
			rage::grcTextureFactoryPC::GetInstance()->LockRenderTarget(0, mDiffuseRT, nullptr);
			rage::grcTextureFactoryPC::GetInstance()->LockRenderTarget(1, mNormalRT, nullptr);
			rage::grcTextureFactoryPC::GetInstance()->LockRenderTarget(2, mSpecularAoRT, nullptr);

			device->SetVertexShader(mSnowVS);
			device->SetPixelShader(mSnowPS);

			device->SetVertexDeclaration(mQuadVertexDecl);
			device->SetStreamSource(0, mQuadVertexBuffer, 0, sizeof(VertexFormat));

			rage::Vector4 projParams;
			float a = currGrcViewport->mFarClip / (currGrcViewport->mFarClip - currGrcViewport->mNearClip);
			projParams.x = currGrcViewport->field_2CC;
			projParams.y = currGrcViewport->field_2C8;
			projParams.z = 1.0f / (0.0f - (a * currGrcViewport->mNearClip));
			projParams.w = projParams.z * a;

			device->SetVertexShaderConstantF(0, &projParams.x, 1);
			device->SetVertexShaderConstantF(1, &currGrcViewport->mViewInverseMatrix[0][0], 4);

			rage::Vector4 threshold;
			if(!HasSnow(currWeather) && HasSnow(nextWeather))
				threshold.y = CWeather::GetNextWeatherPercentage();
			else if(HasSnow(currWeather) && !HasSnow(nextWeather))
				threshold.y = 0.9999f - CWeather::GetNextWeatherPercentage();
			else if(!HasSnow(currWeather) && !HasSnow(nextWeather))
				threshold.y = 0.0f;
			else
				threshold.y = 0.9999f;

			mVolumeIntensity = threshold.y * 4.0f * mCfgVolumetricIntensityMultiplier;

			threshold.y = pow(threshold.y, 0.20f);
			threshold.x = max(0.9999f, (threshold.y / (threshold.y + 0.15f)) * 1.15f);

			rage::Vector4 *v1 = (rage::Vector4*)((mHeightRenderPhase + 0x9B0) + sizeof(rage::Vector4) * *mFrameIndex);
			rage::Vector4 *v2 = (rage::Vector4*)((mHeightRenderPhase + 0x9D0) + sizeof(rage::Vector4) * *mFrameIndex);
			rage::Vector4 heightMapTransformMtx[4]
			{
				{v1->x, 0.0f,  0.0f,  0.0f},
				{0.0f,  v1->y, 0.0f,  0.0f},
				{0.0f,  0.0f,  v1->z, 0.0f},
				{v2->x, v2->y, v2->z, 1.0f}
			};

			device->SetPixelShaderConstantF(0, &threshold.x, 1);
			device->SetPixelShaderConstantF(1, &projParams.x, 1);
			device->SetPixelShaderConstantF(2, &currGrcViewport->mViewInverseMatrix[0][0], 4);
			device->SetPixelShaderConstantF(6, &heightMapTransformMtx[0].x, 4);

			rage::grcRenderTargetPC *heightRT = *(rage::grcRenderTargetPC**)(mHeightRenderPhase + 0x9A4);

			device->SetTexture(0, mNormalRtCopy->mD3DTexture);
			device->SetTexture(1, mSpecularAoRT->mD3DTexture);
			device->SetTexture(2, mStencilRT->mD3DTexture);
			device->SetTexture(3, mDepthRT->mD3DTexture);
			device->SetTexture(4, mSnowTexture);
			device->SetTexture(5, heightRT->mD3DTexture);

			device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);

			rage::grcDevice::grcResolveFlags resolveFlags {};
			rage::grcTextureFactoryPC::GetInstance()->UnlockRenderTarget(0, &resolveFlags);
			rage::grcTextureFactoryPC::GetInstance()->UnlockRenderTarget(1, &resolveFlags);
			rage::grcTextureFactoryPC::GetInstance()->UnlockRenderTarget(2, &resolveFlags);

			for(uint32_t i = 0; i < 5; i++)
			{
				device->SetSamplerState(i, D3DSAMP_MINFILTER, prevMinFilters[i]);
				device->SetSamplerState(i, D3DSAMP_MAGFILTER, prevMagFilters[i]);
			}
		}
	}
}