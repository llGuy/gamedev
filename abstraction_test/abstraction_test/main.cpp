#include <type_traits>

template <typename T1, typename ... Rest> struct tuple : tuple <Rest...> {
	static constexpr size_t size = sizeof...(Rest) + 1;

	tuple(T1 && p1, Rest && ... rest) : value(std::forward<T1>(p1)), tuple<Rest...>(std::forward<Rest>(rest)...) {}
	T1 value;
};

template <typename T1> struct tuple <T1> {
	static constexpr size_t size = 1;

	tuple(T1 && p) : value(std::forward<T1>(p)) {}
	T1 value;
};

namespace impl {

	template <size_t M, size_t N, typename T1, typename ... Rest> auto get_impl_type_i(tuple<T1, Rest...> const & tup) {
		if constexpr (N == 0) return tup.value;
		else return get_impl_type_i<M, N - 1>(static_cast<tuple<Rest...>>(tup));
	}

	template <typename S, typename T1, typename ... Rest> auto get_impl_type_t(tuple<T1, Rest...> const & tup) {
		if constexpr (std::is_same<S, T1>::value) return tup.value;
		else return get_impl_type_t<S, Rest...>(static_cast<tuple<Rest...>>(tup));
	}

}

template <size_t N, typename T1, typename ... Rest> auto get(tuple<T1, Rest...> & tup) {
	return impl::get_impl_type_i<N, N>(tup);
}

template <typename S, typename T1, typename ... Rest> auto get(tuple <T1, Rest...> & tup) {
	return impl::get_impl_type_t<S>(tup);
}

#include <iostream>

auto main(int argc, char * argv[]) -> int {
	tuple <int, char, double, bool> t(2, 'a', 3.1415, true);

	auto val1 = get<2>(t);
	auto val2 = get<double>(t);

	std::cout << val1 << " " << val2 << std::endl;

	std::cin.get();
}