#pragma once
#include <cstdint>

#include "../Utils.h"

namespace rage
{
	class FunctorBase
	{
	public:
		FunctorBase()
		{
			memset(mMemFunc, 0xAA, 8);
			mCallee = 0;
		}

		FunctorBase(void *callee, void(__fastcall *function)(), void *mf, uint32_t size)
		{
			auto func = (void(__thiscall *)(FunctorBase *, void *, void(__fastcall *)(), void *, uint32_t))Utils::ReadMemory(0x658A0);
			func(this, callee, function, mf, size);
		}

		union
		{
			void(__fastcall *mFunction)();
			uint8_t mMemFunc[8];
		};

		uint32_t mCallee;
	};

	class Functor0 : public FunctorBase
	{
	public:
		Functor0(void *callee, void(__fastcall *function)(), void *mf, uint32_t size)
			: FunctorBase(callee, function, mf, size)
		{
			mThunk = Translator;
		}

	private:
		void(__cdecl *mThunk)(FunctorBase *);

		static void Translator(FunctorBase *functor)
		{
			functor->mFunction();
		}
	};

	static_assert(sizeof(Functor0) == 0x10);
}