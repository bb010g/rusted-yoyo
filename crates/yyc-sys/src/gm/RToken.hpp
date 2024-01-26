#pragma once
#include "Common.hpp"
#include "RValue.hpp"

struct RToken
{
	int kind;
	unsigned int type;
	int ind;
	int ind2;
	RValue value;
	int itemnumb;
	RToken* items;
	int position;
};
