#pragma once

#pragma once


#include <array>
#include <numeric>
#include <utility>
#include "types.h"
#include <glm/glm.hpp>
#include <type_traits>
#include <algorithm>

namespace detail {

	template <typename T1, typename T2, typename ... Args> auto construct(Args ... params) -> T1
	{
		return T1(static_cast<T2>(params)...);
	}

	template <typename ... T> auto make_array(T && ... params)->std::array <typename std::common_type <T...>::type, sizeof... (T)>
	{
		return { std::forward<T>(params)... };
	}

	template <typename C, typename ... T> auto make_array_and_cast(T && ... params)
		->std::array <typename std::common_type <T...>::type, sizeof... (T)>
	{
		return { static_cast<C>(params)... };
	}

	template <typename T, u32 N> auto vec_rand(i32 max) -> glm::vec<N, T, glm::highp>
	{
		using type = glm::vec<N, T, glm::highp>;
		type result;
		for (u32 i = 0; i < N; ++i) result[i] = static_cast<T>(rand() % max);
		return result;
	}

	template <typename F> auto fequ(F a, F b) -> bool
	{
		return fabs(a - b) < 0.00001f;
	}

	template <typename T, std::size_t N, std::size_t ... I> auto random_vec_impl(u32 max, std::index_sequence<I...>) -> glm::vec<N, T, glm::highp>
	{
		return glm::vec<N, T, glm::highp>((I, static_cast<T>(rand() % max))...);
	}

	template <typename T, std::size_t N> auto random_vec(u32 max) -> glm::vec<N, T, glm::highp>
	{
		glm::vec<N, T, glm::highp> result;
		for (u32 i = 0; i < N; ++i)
			result[i] = (T)(rand() % max);

		return result;
	}

	constexpr auto compile_hash(char const * str, u32 size) -> u32
	{
		return ((size ? compile_hash(str, size - 1) : 2166136261u) ^ str[size]) * 16777619u;
	}

	namespace {

		glm::mat4 identity_matrix(1);
		glm::vec4 null_vector(0);
		glm::vec3 up(0, 1, 0);

		auto do_nothing(void) -> void {}

		auto debug(bool condition) -> void
		{
			if (condition)
			{
				do_nothing();
			}
		}
	}

}