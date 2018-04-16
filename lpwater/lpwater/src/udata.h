#pragma once
#include <stdint.h>

enum UDType
	: uint32_t
{
	MAT4,

	VEC3,

	INV
};

struct UDataLoc
{
	UDataLoc(const UDType& t, const char* n)
		: type(t), name(n)
	{
	}
	UDataLoc(const UDType& t, const char* n, const uint32_t& loc)
		: type(t), name(n), location(loc)
	{
	}
	UDType type;
	const char* name;
	uint32_t location;
};