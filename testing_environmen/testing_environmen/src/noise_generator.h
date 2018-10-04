#pragma once

#include <array>
#include "detail.h"
#include <glm/glm.hpp>

using corner_pair = std::array<glm::vec2, 4>;

/* operates on meshes */
class perlin_noise_generator
{
private:
	std::vector<corner_pair> noise_vectors;
	std::vector<corner_pair> corner_positions;

	f32 width, dep;

	f32 factor;
	f32 divisor;
public:
	perlin_noise_generator(f32 w, f32 d, f32 factor, f32 div) 
		: width(w), dep(d), factor(factor), divisor(div)
	{
		prepare_corners();
	}

	auto operator()(glm::vec2 const & pos) const -> f32
	{
		/* check on which square the position is on */
		i32 square_x = static_cast<i32>(pos.x / (width / divisor));
		i32 square_z = static_cast<i32>(pos.y / (dep / divisor));

		i32 square = get_corners_index(square_x, square_z, (i32)divisor);

		glm::vec2 position = pos / factor;

		std::array<f32, 4> noise_values;

		for (u32 i = 0; i < 4; ++i)
		{
			noise_values[i] = glm::dot(noise_vectors[square][i], position - corner_positions[square][i]);
		}

		f32 noise_lerp_x1 = interpolate(glm::vec2(corner_positions[square][0].x, noise_values[0]), 
			glm::vec2(corner_positions[square][1].x, noise_values[1]), position.x);

		f32 noise_lerp_x2 = interpolate(glm::vec2(corner_positions[square][2].x, noise_values[2]), 
			glm::vec2(corner_positions[square][3].x, noise_values[3]), position.x);

		return interpolate(glm::vec2(corner_positions[square][2].y, noise_lerp_x2), 
			glm::vec2(corner_positions[square][0].y, noise_lerp_x1), position.y) * 0.3f;
	}
private:
	template <typename T> auto get_corners_index(T x, T z, T x_divided) const -> i32
	{
		return x + z * x_divided;
	}

	auto prepare_corners(void) -> void
	{
		i32 x_width_divided = static_cast<i32>(divisor);
		i32 z_width_divided = static_cast<i32>(divisor);

		noise_vectors.resize(x_width_divided * z_width_divided);
		corner_positions.resize(x_width_divided * z_width_divided);

		for (i32 z = 0; z < x_width_divided; ++z)
		{
			for (i32 x = 0; x < z_width_divided; ++x)
			{
				i32 index = get_corners_index(x, z, x_width_divided);

				corner_positions[index][0] = glm::vec2(-0.5f, -0.5f);
				corner_positions[index][1] = glm::vec2(x * (width / divisor) + 0.5f, -0.5f);
				corner_positions[index][2] = glm::vec2(-0.5f, z * (dep / divisor) + 0.5f);
				corner_positions[index][3] = glm::vec2(x * (width / divisor) + 0.5f, z * (dep / divisor) + 0.5f);
			}
		}

		for (auto & noises : noise_vectors)
		{
			for (auto & vec : noises)
			{
				vec = glm::normalize(detail::random_vec<f32, 2>(100));
			}
		}
	}

	auto lerp(f32 a, f32 b, f32 dest) -> f32
	{
		return (dest - a) / (b - a);
	}

	auto interpolate(glm::vec2 const & a, glm::vec2 const & b, f32 ref) const -> f32
	{
		return a.y + ((ref - a.x) / (b.x - a.x)) * (b.y - a.y);
	}
};

class static_noise_generator
{
public:
	auto operator()(glm::vec2 const & pos) const -> f32
	{
		return 0.0f;
	}
};