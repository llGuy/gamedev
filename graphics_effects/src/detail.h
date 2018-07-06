#ifndef _DETAIL_H_
#define _DETAIL_H_

#include <array>
#include <type_traits>

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

}

#endif