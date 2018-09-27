#pragma once

#include <new>
#include <array>
#include <utility>

namespace gml { namespace detail {

	template <typename T, std::size_t N, std::size_t ... I> 
	auto construct_with_one(std::array<T, N> & arr, T val, std::index_sequence<I...>) -> void
	{
		new(arr.data()) std::array<T, N> { (I, val)... };
	}

}  }