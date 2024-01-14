#pragma once
#include <Windows.h>
#include <injector/injector.hpp>

namespace Utils
{
	template<typename T>
	inline void WriteMemory(uint32_t address, T value)
	{
		static uint32_t baseAddress = 0;
		if(!baseAddress)
		{
			baseAddress = (uint32_t)GetModuleHandle(L"GTAIV.exe");
		}

		injector::WriteMemory(baseAddress + address, value);
	}

	template<typename T>
	inline void WriteMemory(uint32_t address, T value, uint32_t size)
	{
		static uint32_t baseAddress = 0;
		if(!baseAddress)
		{
			baseAddress = (uint32_t)GetModuleHandle(L"GTAIV.exe");
		}

		injector::WriteMemoryRaw(baseAddress + address, value, size, true);
	}

	template<typename T>
	inline void WriteCall(uint32_t address, T function)
	{
		static uint32_t baseAddress = 0;
		if(!baseAddress)
		{
			baseAddress = (uint32_t)GetModuleHandle(L"GTAIV.exe");
		}

		injector::MakeCALL(baseAddress + address, function);
	}

	inline void *ReadMemory(uint32_t address)
	{
		static uint32_t baseAddress = 0;
		if(!baseAddress)
		{
			baseAddress = (uint32_t)GetModuleHandle(L"GTAIV.exe");
		}

		return (void *)(baseAddress + address);
	}
}