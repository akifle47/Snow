#define _CRT_SECURE_NO_WARNINGS

#include "SnowRenderer.h"

void (__cdecl *CRenderPhaseDeferredLighting_LightsToScreen__BuildRenderListO)() = nullptr;
void (__cdecl *CopyLightO)() = nullptr;

void __declspec(naked) CRenderPhaseDeferredLighting_LightsToScreen__BuildRenderListH()
{
	_asm
	{
		push ecx
		call SnowRenderer::BuildRenderList

		pop ecx
		call CRenderPhaseDeferredLighting_LightsToScreen__BuildRenderListO

		ret
	}
}

void __declspec(naked) CopyLightH()
{
	_asm
	{
		push [esp+0x4]
		call CopyLightO

		push eax
		call SnowRenderer::OnAfterCopyLight
		pop eax

		ret 0x4
	}
}

BOOL WINAPI DllMain(HINSTANCE, DWORD fdwReason, LPVOID)
{
	if(fdwReason == DLL_PROCESS_ATTACH)
	{
		if(*(uint32_t *)Utils::ReadMemory(0x208C34) != 0x404B100F)
		{
			MessageBox(0, L"Only version 1.0.8.0 is supported", L"Snow.asi", MB_ICONERROR | MB_OK);
			return false;
		}

		CRenderPhaseDeferredLighting_LightsToScreen__BuildRenderListO = *(void(__cdecl **)())Utils::ReadMemory(0x9E4874);
		Utils::WriteMemory(0x9E4874, CRenderPhaseDeferredLighting_LightsToScreen__BuildRenderListH);

		CopyLightO = (void(__cdecl *)())Utils::ReadMemory(0x62A7F0);
		Utils::WriteCall(0x62DAB7, CopyLightH);
		Utils::WriteCall(0x62DA71, CopyLightH);
	}

	return true;
}