#ifndef _CELL_H_
#define _CELL_H_

#include <string>

template <int8_t LifeSpan>
class cell
{
public:
	static constexpr int8_t life_span = LifeSpan;

	cell(void)
		: state(0)
	{
	}
	auto live(void) -> void
	{
		state = 1;
	}
	auto alive(void) -> bool
	{
		return state == 1;
	}
	auto to_string(void) -> std::string 
	{
		return std::to_string(state);
	}
	auto die(void) -> void
	{
		state = 0;
	}
	operator int() const 
	{
		return static_cast<int32_t>(state);
	}
private:
	int8_t state;
};

using default_cell = cell<5>;

#endif