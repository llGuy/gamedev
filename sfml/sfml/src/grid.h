#ifndef _GRID_H_
#define _GRID_H_

#include <iostream>
#include <array>
#include <memory>
#include "cell.h"

template <uint32_t X, uint32_t Y>
class grid
{
public:
	static constexpr uint32_t max_cells = X * Y;

	using generation = std::array<default_cell, max_cells>;

	grid(void) = default;

	auto generate(void) -> void
	{
		for (uint32_t i = 0; i < X * Y; ++i)
		{
			if (rand() % 2)
			{
				current[i].live();
				scratch[i].live();
			}
		}
	}
	auto copy(generation & src, generation & dst) -> void
	{
		std::copy(src.begin(), src.end(), dst.begin());
	}
	auto update(void) -> void
	{
		auto do_nothing = [] { /* do absolutely nothing */ };
		for (uint32_t y = 0; y < Y - 0; ++y)
		{
			for (uint32_t x = 0; x < X - 0; ++x)
			{
				default_cell & c = current[index(x, y)];
				default_cell & s = scratch[index(x, y)];

				uint32_t num_neighbors = count_neighbors(x, y);
				
				if (num_neighbors <= 1) s.die();
				else if (num_neighbors == 2) do_nothing();
				else if (num_neighbors == 3) s.live();
				else if (num_neighbors >= 4) s.die();
			}
		}
		copy(scratch, current);
	}
	auto print(void)
	{
		for (uint32_t y = 0; y < Y; ++y)
		{
			for (uint32_t x = 0; x < X; ++x)
			{
				std::cout << (int)current[index(x, y)] << ' ';
			}
			std::cout << std::endl;
		}
	}
public:
	auto begin(void) 
	{
		return current.begin();
	}
	auto end(void) 
	{
		return current.end();
	}
private:
	auto index(uint32_t x, uint32_t y) -> uint32_t
	{
		return x + y * X;
	}
	auto count_neighbors(uint32_t center_x, uint32_t center_y) -> uint32_t
	{
		static constexpr int32_t x_offsets[] { -1, 0, +1, +1, +1, 0, -1, -1 };
		static constexpr int32_t y_offsets[] { -1, -1, -1, 0, +1, 1, +1, +0 };

		uint32_t res = 0;
		for (int32_t i = 0; i < 8; i++)
		{
			int32_t x = center_x + x_offsets[i];
			int32_t y = center_y + y_offsets[i];

			if ((x >= 0 && x < X) && (y >= 0 && y < Y))
				if (current[index(x, y)].alive()) ++res;
		}

		return res;
	}
private:
	generation current;
	generation scratch;
};

using default_grid = grid<30, 30>;

#endif