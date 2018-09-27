#include <iostream>
//#include <emmintrin.h>
#include <glm/glm.hpp>

#include <array>
#include <glm/glm.hpp>
#include <chrono>

class timer
{
public:
	timer(void) = default;
	auto start(void) -> void;
	auto reset(void) -> void;
	auto elapsed(void) -> float;
private:
	std::chrono::high_resolution_clock::time_point current_time;
};

auto timer::start(void) -> void
{
	current_time = std::chrono::high_resolution_clock::now();
}
auto timer::reset(void) -> void
{
	current_time = std::chrono::high_resolution_clock::now();
}
auto timer::elapsed(void) -> float
{
	std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> seconds = now - current_time;
	float count = seconds.count();
	return count;
}



auto sse_test(float * a, int n) -> void
{
	float res;

	__m128 * f4 = (__m128 *)a;
	__m128 max_val = _mm_setzero_ps();

	for (int i = 0; i < n / 4; ++i)
	{
		max_val = _mm_max_ps(max_val, f4[i]);
	}

	for (int i = 0; i < 3; ++i)
	{
		max_val = _mm_max_ps(max_val, _mm_shuffle_ps(max_val, max_val, 0x93));
	}

	_mm_store_ss(&res, max_val);
}

auto sum_sse(float * a, int n) -> void
{
	float res = 0.0f;

	__m128 * v4 = (__m128 *)a;
	__m128 vec_sum = _mm_setzero_ps();

	for (int i = 0; i < n / 4; ++i)
	{
		vec_sum = _mm_add_ps(vec_sum, v4[i]);
	}

	vec_sum = _mm_hadd_ps(vec_sum, vec_sum);
	vec_sum = _mm_hadd_ps(vec_sum, vec_sum);
}


template <typename T, std::size_t N> class __declspec(align(16)) vec
{
private:
	using type = vec<T, N>;

	T v[N];
public:
	vec(void) = default;

	template <typename T1, typename ... Ts> vec(T1 v1, Ts ... vs)
		: v{ static_cast<T>(v1), static_cast<T>(vs)... }
	{
	}

	vec(T i)
	{
		for (std::size_t i = 0; i < N; ++i)
			v[i] = i;
	}

	auto add_normal(type const & other) -> type
	{
		type new_vec;
		for (std::size_t i = 0; i < N; ++i)
		{
			new_vec.v[i] = v[i] + other.v[i];
		}

		return new_vec;
	}

	auto add_sse(type const & other) -> type
	{
		__m128 * this_sse = (__m128 *)v;
		__m128 * oth_sse = (__m128 *)other.v;

		type temp;
		_mm_store_ps(temp.v, _mm_add_ps(*this_sse, *oth_sse));

		return temp;
	}

	auto operator[](std::size_t index) -> T &
	{
		return v[index];
	}

	auto data(void) -> T *
	{
		return v;
	}
};


template <typename T, std::size_t N> auto dot(vec<T, N> & a, vec<T, N> & b) -> T
{
	__m128 * v4_1 = (__m128 *)(a.data());
	__m128 * v4_2 = (__m128 *)(b.data());

	__m128 res = _mm_mul_ps(*v4_1, *v4_2);

	res = _mm_hadd_ps(res, res);
	res = _mm_hadd_ps(res, res);

	float scalar;

	_mm_store_ss(&scalar, res);

	return scalar;
}


template <typename T, std::size_t N> auto dot_normal(vec<T, N> & a, vec<T, N> & b) -> T
{
	float scalar = 0.0f;

	for (std::size_t i = 0; i < N; ++i)
		scalar += a[i] * b[i];

	return scalar;
}


using vec4 = vec<float, 4>;


auto test_add(void) -> void
{
	using namespace std::chrono;

	high_resolution_clock::time_point n = high_resolution_clock::now();

	for (std::size_t i = 0; i < 1000; ++i)
	{
		vec4 a(i);
		vec4 b(i + 1);

		dot(a, b);
	}

	std::cout << (high_resolution_clock::now() - n).count() << std::endl;

	high_resolution_clock::time_point n2 = high_resolution_clock::now();

	for (std::size_t i = 0; i < 1000; ++i)
	{
		glm::vec4 a(i);
		glm::vec4 b(i + 1);

		glm::dot(a, b);
	}

	std::cout << (high_resolution_clock::now() - n2).count() << std::endl;

	std::cout << std::endl << std::endl;
}


auto main(void) -> int
{
	vec4 a(1.0f, 0.0f, 0.0f, 0.0f);
	vec4 b(0.0f, 1.0f, 0.0f, 0.0f);

	float scalar = dot(a, b);

	std::cout << scalar << std::endl;

	for(std::size_t i = 0; i < 100; ++i)
		test_add();
	
	float v[2]{ 4, 4 };

	__m128 s = _mm_load_ps(v);

	__m128 res = _mm_sqrt_ps(s);

	float r[2];

	_mm_store_ps(r, res);


	std::cin.get();
}