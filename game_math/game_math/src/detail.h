#pragma once

#include <new>
#include <array>
#include <utility>

namespace detail {

	template <typename T, std::size_t N, std::size_t ... I> 
	auto construct_array_with_one(std::array<T, N> & dest, T value, std::index_sequence<I...>)
	{
		new(dest.data()) std::array<T, N>( (I, value)... );
	}

}