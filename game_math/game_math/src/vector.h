#pragma once

#include <array>
#include <utility>
#include <iostream>
#include <intrin.h>
#include <emmintrin.h>

#include "detail.h"

namespace gml {

	template <typename T> using scalar = T;

	template <typename T, std::size_t N> class __declspec(align(16)) vector
	{
	private:
		std::array<scalar<T>, N> v;
	public:
		vector(void) = default;

		vector(T value)
		{
			detail::construct_with_one(v, value, std::make_index_sequence<N>());
		}

		template <typename ... Ts> vector(T && v1, T && v2, Ts && ... vs)
			: v{ v1, v2, vs... }
		{
		}
	public:
		/* access */
		auto data(void) -> scalar<T> *
		{
			return v.data();
		}

		auto data(void) const -> scalar<T> const *
		{
			return v.data();
		}

		auto operator[](std::size_t index) -> scalar<T> &
		{
			return v[index];
		}
	public:
		/* basic math operations */
		auto operator=(vector<T, N> const & other) -> vector<T, N>
		{
			__m128 other_sse = _mm_load_ps(other.data());
			_mm_store_ps(v.data(), other_sse);
			return *this;
		}

		auto operator+(vector<T, N> & other) -> vector<T, N>
		{
			vector<T, N> new_v;

			__m128 a = _mm_load_ps(data());
			__m128 b = _mm_load_ps(other.data());

			_mm_store_ps(new_v.data(), _mm_add_ps(a, b));

			return new_v;
		}

		auto operator-(vector<T, N> & other) -> vector<T, N>
		{
			vector<T, N> new_v;

			__m128 a = _mm_load_ps(data());
			__m128 b = _mm_load_ps(other.data());

			_mm_store_ps(new_v.data(), _mm_sub_ps(a, b));

			return new_v;
		}
	};


	template <typename T, std::size_t N> auto dot(vector <T, N> const & a, vector<T, N> const & b) -> scalar<T>
	{
		scalar<T> __declspec(align(16)) res_scalar = 0.0f;

		__m128 a_s = _mm_load_ps(a.data());
		__m128 b_s = _mm_load_ps(b.data());

		__m128 res = _mm_mul_ps(a_s, b_s);

		res = _mm_hadd_ps(res, res);
		res = _mm_hadd_ps(res, res);
		
		_mm_store_ss(&res_scalar, res);

		return res_scalar;
	}
}