#pragma once

#include <array>
#include <vector>
#include <numeric>
#include <utility>
#include "types.h"
#include <glm/glm.hpp>
#include <type_traits>
#include <algorithm>
#include <glm/gtc/quaternion.hpp>

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

	template <typename T> auto copy_from_pointer(T * ptr, std::size_t size_bytes) -> std::vector<T>
	{
		std::size_t size = size_bytes / sizeof(T);

		std::vector<T> arr;
		arr.reserve(size);

		for (std::size_t i = 0; i < size; ++i)
			arr.push_back(ptr[i]);

		return arr;
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

		auto convert_u32b_to_vec4_color(u32 color) -> glm::vec4
		{
			f32 r = static_cast<f32>((color >> 24) & 0xff) / 255.0f;
			f32 g = static_cast<f32>((color >> 16) & 0xff) / 255.0f;
			f32 b = static_cast<f32>((color >> 8) & 0xff) / 255.0f;
			f32 a = static_cast<f32>((color >> 0) & 0xff) / 255.0f;
			
			return glm::vec4(r, g, b, a);
		}

		auto from_matrix(glm::mat4 const & matrix) -> glm::quat
		{
			f32 w, x, y, z;

			f32 diagonal = matrix[0][0] + matrix[1][1] + matrix[2][2];
			if (diagonal > 0) 
			{
				float w4 = (float)(sqrtf(diagonal + 1.0f) * 2.0f);
				w = w4 / 4.0f;
				x = (matrix[2][1] - matrix[1][2]) / w4;
				y = (matrix[0][2] - matrix[2][0]) / w4;
				z = (matrix[1][0] - matrix[0][1]) / w4;
			}
			else if ((matrix[0][0] > matrix[1][1]) && (matrix[0][0] > matrix[2][2])) 
			{
				float x4 = (float)(sqrtf(1.0f + matrix[0][0] - matrix[1][1] - matrix[2][2]) * 2.0f);
				w = (matrix[2][1] - matrix[1][2]) / x4;
				x = x4 / 4.0f;
				y = (matrix[0][1] + matrix[1][0]) / x4;
				z = (matrix[0][2] + matrix[2][0]) / x4;
			}
			else if (matrix[1][1] > matrix[2][2]) 
			{
				float y4 = (float)(sqrtf(1.0f + matrix[1][1] - matrix[0][0] - matrix[2][2]) * 2.0f);
				w = (matrix[0][2] - matrix[2][0]) / y4;
				x = (matrix[0][1] + matrix[1][0]) / y4;
				y = y4 / 4.0f;
				z = (matrix[1][2] + matrix[2][1]) / y4;
			}
			else 
			{
				float z4 = (float)(sqrtf(1.0f + matrix[2][2] - matrix[0][0] - matrix[1][1]) * 2.0f);
				w = (matrix[1][0] - matrix[0][1]) / z4;
				x = (matrix[0][2] + matrix[2][0]) / z4;
				y = (matrix[1][2] + matrix[2][1]) / z4;
				z = z4 / 4.0f;
			}
			return glm::quat{ w, x, y, z };
		}
	}

}

using hashed_string_value_type = u32;

struct hashed_string
{
	char const * str;
	unsigned int length;
	unsigned int hashed_value;
};

static auto create_hashed_string(std::string const & str) -> hashed_string
{
	hashed_string ret{ str.c_str(), str.length(), ((str.length() ? detail::compile_hash(str.c_str(), str.length() - 1) : 2166136261u) ^ str.c_str()[str.length()]) * 16777619u };

	return ret;
}

constexpr auto operator""_hash(char const * s, std::size_t count) -> hashed_string
{
	hashed_string ret{ s, count, ((count ? detail::compile_hash(s, count - 1) : 2166136261u) ^ s[count]) * 16777619u };

	return ret;
}