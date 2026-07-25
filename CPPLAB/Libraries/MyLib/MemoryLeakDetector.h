/*
	@file   MemoryLeakDetector.h
	@brief  ƒƒ‚ƒŠƒŠ[ƒN‚ÌŒŸoŠí
*/
#pragma once

#if defined(_DEBUG)

#define _CRTDBG_MAP_ALLOC

#include <crtdbg.h>

namespace mylib
{
	inline void MemoryLeakDetector()
	{
		_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);
	}
}

#define MY_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)

#else

namespace mylib
{
	inline void MemoryLeakDetector()
	{}
}

#define MY_NEW new

#endif