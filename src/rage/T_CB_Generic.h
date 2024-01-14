#pragma once
#include <cstdint>
#include "../Utils.h"

namespace rage
{
	class CBaseDC
	{
	private:
		void *_vft;

	public:

		void *operator new(uint32_t size, uint32_t unused = 0)
		{
			auto func = (CBaseDC *(__cdecl *)(uint32_t, uint32_t))Utils::ReadMemory(0x45E390);
			return func(size, unused);
		}

		//just to shut the compiler up
		void operator delete(void *ptr, uint32_t unused)
		{}

		void operator delete(void *ptr)
		{}

		void Init()
		{
			auto func = (void(__thiscall *)(CBaseDC *))Utils::ReadMemory(0x75920);
			func(this);
		};

	private:
		uint32_t mCommandIndex : 14;
		uint32_t mCommandSize : 11;
		uint32_t mInstructionId : 7;
	};

	static_assert(sizeof(CBaseDC) == 0x8);

	template<typename T>
	class T_CB_Generic_NoArgs : public CBaseDC
	{
	public:
		T_CB_Generic_NoArgs(T function)
		{
			auto func = (T_CB_Generic_NoArgs *(__thiscall *)(T_CB_Generic_NoArgs *, T))Utils::ReadMemory(0xBDBE0);
			func(this, function);
		}

		T mFunction;
	};
}
