#pragma once
#include "Common.hpp"

template<typename T>
struct RefThing
{
	T thing;
	int32_t ref_count;
	int32_t size;
};

typedef RefThing<const char*> RefString;
