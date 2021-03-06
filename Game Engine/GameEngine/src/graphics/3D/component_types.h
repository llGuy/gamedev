#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "../../api/api.h"

struct vertex_buffer_component { buffer value; };
struct vertices_component { std::vector<glm::vec3> vertices; };
struct normal_buffer_component { buffer value; };
struct color_buffer_component { buffer value; };
struct texture_buffer_component { buffer value; };
struct index_buffer_component { buffer value; };
struct texture_component { texture value; u32 point; };
struct weights_buffer_component { buffer value; };
struct joint_ids_buffer_component { buffer value; };
/* in 3D, there will basically always be an index buffer */