#pragma once

#include "quad_3D.h"
#include "program.h"
#include "resources.h"
#include "render_func.h"
#include <glm/gtc/type_ptr.hpp>

#define DEPTH 0.0f
#define WIDTH 1.0f
#define HEIGHT 1.0f

class gradient_sky
{
private:
	program shaders;
	quad_3D sky_quad;
public:
	gradient_sky()
		: sky_quad(glm::vec3(-WIDTH, -HEIGHT, DEPTH),
			glm::vec3(-WIDTH, HEIGHT, DEPTH),
			glm::vec3(WIDTH, -HEIGHT, DEPTH),
			glm::vec3(WIDTH, HEIGHT, DEPTH))
	{
	}

	auto create(resource_handler & rh, glm::mat4 & projection, f32 fov, f32 aspect) -> void
	{
		f32 far = 200.0f;

		shaders.attach(shader(GL_VERTEX_SHADER, "sky/vsh.shader"));
		shaders.attach(shader(GL_FRAGMENT_SHADER, "sky/fsh.shader"));
		shaders.link("vertex_position", "vertex_color");
		shaders.get_uniform_locations("projection_matrix", "camera_direction", "top", "bottom", "model_matrix");
		shaders.bind();
		glm::vec4 blue(1.0f, 1.0f, 1.0f, 1.0f);
		//glm::vec4 yellow = glm::mix(blue, glm::vec4(1.0f, 1.0f, 0.0f, 1.0f), 0.6f);
		glm::vec4 yellow(1.0f);

		shaders.send_uniform_mat4(0, glm::value_ptr(projection), 1);
		shaders.send_uniform_vec4(2, glm::value_ptr(blue), 1);
		shaders.send_uniform_vec4(3, glm::value_ptr(yellow), 1);


		f32 far_width = 2.0f * far * tan(glm::radians(fov));
		f32 far_height = far_width / aspect;


		glm::mat4 model_matrix = glm::translate(glm::vec3(0.0f, 0.0f, -far)) 
			* glm::scale(glm::vec3(far_width / 2.0f, far_height / 2.0f, 1.0f));

		shaders.send_uniform_mat4(4, glm::value_ptr(model_matrix), 1);


		glm::vec3 colors [] 
		{
			yellow, blue, yellow, blue
		};

		sky_quad.create(rh);

		unbind_vertex_layouts();
	}

	auto render(glm::vec3 & cam_dir) -> void
	{
		shaders.bind();

		shaders.send_uniform_vec3(1, glm::value_ptr(cam_dir), 1);

		render_model(sky_quad, GL_TRIANGLE_STRIP);
	}
};