
#ifndef _DETAIL_H_
#define _DETAIL_H_

#include <array>
#include <glm/glm.hpp>
#include <type_traits>
#include <algorithm>

namespace detail {

	template <typename Result, typename Type, typename ... Params>
	Result construct(Params ... params)
	{
		return Result(static_cast<Type>(params)...);
	}

	template <typename ... T>
	auto make_array(T && ... params) -> std::array <typename std::common_type <T...>::type, sizeof... (T)>
	{
		return { std::forward<T>(params)... };
	}

	template <typename T, uint32_t N>
	auto vec_rand(int32_t max) -> glm::vec<N, T, glm::highp>
	{
		using type = glm::vec<N, T, glm::highp>;
		type result;
		for (uint32_t i = 0; i < N; ++i) result [i] = static_cast<T>(rand() % max);
		return result;
	}

}

#endif
