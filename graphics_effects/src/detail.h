#ifndef _DETAIL_H_
#define _DETAIL_H_

namespace detail {

	template<typename Result, typename Type, typename ... Params>
	Result construct(Params ... params)
	{
		return Result(static_cast<Type>(params)...);
	}

}

#endif