#pragma once

namespace rage
{
	class datBase
	{
	protected:
		uint32_t *_vft;
	};

	class pgBase : public datBase
	{
		uint32_t mBlockMap;
	};

	static_assert(sizeof(pgBase) == 0x8);
}