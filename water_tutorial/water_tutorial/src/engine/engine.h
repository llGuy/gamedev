#ifndef ENGINE_HEADER
#define ENGINE_HEADER

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>

#include "framebuffer\framebuffer.h"
#include "../shader/shprogram.h"
#include "player.h"
#include "quad.h"
#include "3DObj.h"
#include "data.h"

class Engine
{
public:
	explicit Engine(const uint32_t& width, const uint32_t& height)
	{
		m_projectionMatrix = glm::perspective(glm::radians(60.0f), (float)width / height, 0.1f, 50.0f);
	}
	void Init(void)
	{
		m_quad.Init();
		m_above.Init(glm::vec3(3.0f, -0.5f, 3.0f));
		m_below.Init(glm::vec3(-3.0f, -2.5f, -3.0f));

		m_program.Init("res\\vsh.shader", "res\\fsh.shader", "INV");
		m_program.Compile();
		std::vector<const char*> attribs({"vertex_position", "vertex_colour"});
		m_program.Link(attribs);
		GetUniformLocations();

		m_fbos.Init();
		m_plane = glm::vec4(0.0f, -1.0f, 0.0f, 1.0f);
	}
	Player& Pl(void) { return m_player; }
	void MouseUpdate(float x, float y) { m_player.Look(glm::vec2(x, y), 0.02f); };

	void Draw(void)
	{
		glEnable(GL_CLIP_DISTANCE0);
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		m_program.UseProgram();
		m_viewMatrix = glm::lookAt(m_player.Position(), m_player.Position() + m_player.Direction(), m_player.Up());
		glUniformMatrix4fv(m_locations.projLocation, 1, GL_FALSE, &m_projectionMatrix[0][0]);
		glUniformMatrix4fv(m_locations.viewLocation, 1, GL_FALSE, &m_viewMatrix[0][0]);
		glUniform4fv(m_locations.planeLocation, 1, &m_plane[0]);

		glBindVertexArray(m_quad.VAO());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_quad.BufferID());
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (void*)(sizeof(float) * 3 * 4 * 2));
		glBindVertexArray(m_above.VAO());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_above.BufferID());
		glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_SHORT, m_above.Offset());
		glBindVertexArray(m_below.VAO());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_below.BufferID());
		glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_SHORT, m_below.Offset());

		m_fbos.BindReflection();
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUniformMatrix4fv(m_locations.projLocation, 1, GL_FALSE, &m_projectionMatrix[0][0]);
		glUniformMatrix4fv(m_locations.viewLocation, 1, GL_FALSE, &m_viewMatrix[0][0]);
		glBindVertexArray(m_quad.VAO());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_quad.BufferID());
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (void*)(sizeof(float) * 3 * 4 * 2));
		glBindVertexArray(m_above.VAO());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_above.BufferID());
		glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_SHORT, m_above.Offset());
		glBindVertexArray(m_below.VAO());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_below.BufferID());
		glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_SHORT, m_below.Offset());
		m_fbos.Unbind();
	}

	void GetUniformLocations(void)
	{
		m_locations.projLocation = glGetUniformLocation(m_program.ProgramID(), "projection_matrix");
		m_locations.viewLocation = glGetUniformLocation(m_program.ProgramID(), "view_matrix");

		m_locations.planeLocation = glGetUniformLocation(m_program.ProgramID(), "plane");
	}
private:
	glm::mat4 m_projectionMatrix;
	Quad m_quad;
	D3Obj m_above;
	D3Obj m_below;

	Framebuffers m_fbos;
	SHProgram m_program;
	Player m_player;
	glm::mat4 m_viewMatrix;

	Locs m_locations;

	glm::vec4 m_plane;
};

#endif