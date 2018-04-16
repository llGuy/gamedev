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
#include "gui.h"

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

		m_programGUI.Init("res\\guivsh.shader", "res\\guifsh.shader", "INV");
		m_programGUI.Compile();
		std::vector<const char*> attribs2({ "vertex_position", "texture_coordinate" });
		m_programGUI.Link(attribs2);

		m_waterShader.Init("res\\watervsh.shader", "res\\waterfsh.shader", "INV");
		m_waterShader.Compile();
		std::vector<const char*> attribs3({"vertex_position", "texture_coordinate"});
		m_waterShader.Link(attribs3);
		GetUniformLocations();

		m_fbos.Init();
		m_gui.Init(glm::vec2(0.0f, 0.0f));
		m_gui2.Init(glm::vec2(0.0f, -1.0f));
		m_planeDefault = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
		m_plane = glm::vec4(0.0f, 1.0f, 0.0f, 0.05f);
		m_plane2 = glm::vec4(0.0f, -1.0f, 0.0f, 0.05f);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_fbos.TextureIDReflection());
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_fbos.TextureIDRefraction());
	}
	Player& Pl(void) { return m_player; }
	void MouseUpdate(float x, float y) { m_player.Look(glm::vec2(x, y), 0.02f); };

	void Draw(void)
	{
		glEnable(GL_CLIP_DISTANCE0);
		m_program.UseProgram();
		m_viewMatrix = glm::lookAt(m_player.Position(), m_player.Position() + m_player.Direction(), m_player.Up());
		glUniformMatrix4fv(m_locations.projLocation, 1, GL_FALSE, &m_projectionMatrix[0][0]);
		glUniformMatrix4fv(m_locations.viewLocation, 1, GL_FALSE, &m_viewMatrix[0][0]);
		glUniform4fv(m_locations.planeLocation, 1, &m_planeDefault[0]);

		DrawModels(true);
		m_program.UseProgram();
		glUniformMatrix4fv(m_locations.projLocation, 1, GL_FALSE, &m_projectionMatrix[0][0]);
		glUniformMatrix4fv(m_locations.viewLocation, 1, GL_FALSE, &m_viewMatrix[0][0]);
		glUniform4fv(m_locations.planeLocation, 1, &m_planeDefault[0]);

		glUniform4fv(m_locations.planeLocation, 1, &m_plane[0]);
		float distance = 2.0f * (m_player.Position().y - 0.0f/*water height*/);
		glm::vec3 position = glm::vec3(m_player.Position().x, m_player.Position().y - distance, m_player.Position().z);
		glm::mat4 underneathWater = glm::lookAt(position, position + m_player.Direction(), glm::vec3(0.0f, 1.0f, 0.0f));
		//glm::mat4 underneathWater = glm::lookAt(m_player.Position(), m_player.Position() + m_player.Direction());
		glUniformMatrix4fv(m_locations.viewLocation, 1, GL_FALSE, &underneathWater[0][0]);
		m_fbos.BindReflection();
		DrawModels(false);
		m_fbos.Unbind();

		//m_program.UseProgram();
		//glUniformMatrix4fv(m_locations.projLocation, 1, GL_FALSE, &m_projectionMatrix[0][0]);
		//glUniformMatrix4fv(m_locations.viewLocation, 1, GL_FALSE, &m_viewMatrix[0][0]);
		glUniform4fv(m_locations.planeLocation, 1, &m_plane2[0]);
		glUniformMatrix4fv(m_locations.viewLocation, 1, GL_FALSE, &m_viewMatrix[0][0]);
		m_fbos.BindRefraction();
		DrawModels(false);
		m_fbos.Unbind();

		/*m_programGUI.UseProgram();
		glUniformMatrix4fv(m_guiProjMatLoc, 1, GL_FALSE, &m_projectionMatrix[0][0]);
		glBindTexture(GL_TEXTURE_2D, m_fbos.TextureIDReflection());
		glBindVertexArray(m_gui.VAO());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_gui.BufferID());
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, m_gui.Offset());*/

		/*glBindTexture(GL_TEXTURE_2D, m_fbos.TextureIDRefraction());
		glBindVertexArray(m_gui2.VAO());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_gui2.BufferID());
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, m_gui2.Offset());*/

		//m_program.UseProgram();
	}
	void DrawModels(bool b)
	{
		glClearColor(0.0f, 0.0f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glBindVertexArray(m_above.VAO());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_above.BufferID());
		glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_SHORT, m_above.Offset());
		glBindVertexArray(m_below.VAO());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_below.BufferID());
		glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_SHORT, m_below.Offset());

		if (b)
		{
			/*m_waterShader.UseProgram();
			glUniformMatrix4fv(m_locationsWSH.projLocation, 1, GL_FALSE, &m_projectionMatrix[0][0]);
			glUniformMatrix4fv(m_locationsWSH.viewLocation, 1, GL_FALSE, &m_viewMatrix[0][0]);
			//glUniform4fv(m_locations.planeLocation, 1, &m_planeDefault[0]);
			glBindTexture(GL_TEXTURE0, m_fbos.TextureIDReflection());
			glBindTexture(GL_TEXTURE1, m_fbos.TextureIDRefraction());
			glBindVertexArray(m_quad.VAO());
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_quad.BufferID());
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (void*)(sizeof(float) * 3 * 4 + sizeof(float) * 2 * 4));*/
		}
	}

	void GetUniformLocations(void)
	{
		m_locations.projLocation = glGetUniformLocation(m_program.ProgramID(), "projection_matrix");
		m_locations.viewLocation = glGetUniformLocation(m_program.ProgramID(), "view_matrix");

		m_locations.planeLocation = glGetUniformLocation(m_program.ProgramID(), "plane");
		m_guiProjMatLoc = glGetUniformLocation(m_programGUI.ProgramID(), "projection_matrix");


		m_locations.projLocation = glGetUniformLocation(m_waterShader.ProgramID(), "projection_matrix");
		m_locations.viewLocation = glGetUniformLocation(m_waterShader.ProgramID(), "view_matrix");

		m_reflecLoc = glGetUniformLocation(m_waterShader.ProgramID(), "reflection_texture");
		m_refracLoc = glGetUniformLocation(m_waterShader.ProgramID(), "refraction_texture");
	}
private:
	glm::mat4 m_projectionMatrix;
	Quad m_quad;
	D3Obj m_above;
	D3Obj m_below;
	GUI m_gui;	
	GUI m_gui2;

	Framebuffers m_fbos;
	SHProgram m_program;
	SHProgram m_waterShader;
	SHProgram m_programGUI;
	Player m_player;
	glm::mat4 m_viewMatrix;

	Locs m_locations;
	Locs m_locationsWSH;
	uint32_t m_guiProjMatLoc;

	glm::vec4 m_planeDefault;
	glm::vec4 m_plane;
	glm::vec4 m_plane2;

	uint32_t m_reflecLoc, m_refracLoc;
};

#endif