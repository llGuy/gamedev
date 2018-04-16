#pragma once
#include <GL/glew.h>
#include <glm/gtx/transform.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include "terrain.h"
#include "camerae.h"
#include "program.h"
#include "water_quad.h"
#include "framebuffer.h"
#include "gui.h"

class Engine
{
public:
	Engine(int32_t width, int32_t height)
		: m_fbos(width, height)
	{
		glEnable(GL_DEPTH_TEST);
		m_projectionMatrix = glm::perspective(glm::radians(70.0f), (float)width / height, 0.1f, 100.0f);

		m_terrainShader.Init(ShaderBase{ "src\\terrainvsh.shader", GL_VERTEX_SHADER }, ShaderBase{ "src\\terrainfsh.shader", GL_FRAGMENT_SHADER });
		m_terrainShader.Compile();
		m_terrainShader.Link("vertex_position");
		m_terrainShader.GetUniformLocations(UDataLoc(UDType::MAT4, "projection_matrix"),
			UDataLoc(UDType::MAT4, "view_matrix"));

		m_waterShader.Init(ShaderBase{ "src\\watervsh.shader", GL_VERTEX_SHADER }, ShaderBase{ "src\\waterfsh.shader", GL_FRAGMENT_SHADER });
		m_waterShader.Compile();
		m_waterShader.Link("vertex_position", "vertex_color");
		m_waterShader.GetUniformLocations(UDataLoc(UDType::MAT4, "projection_matrix"),
			UDataLoc(UDType::MAT4, "view_matrix"));

		m_guiShader.Init(ShaderBase{ "src\\guivsh.shader", GL_VERTEX_SHADER }, ShaderBase{ "src\\guifsh.shader", GL_FRAGMENT_SHADER });
		m_guiShader.Compile();
		m_guiShader.Link("vertex_position", "texture_coordinate");
		m_guiShader.GetUniformLocations();

		glActiveTexture(GL_TEXTURE0);
	}
	void Render(void)
	{
		
		glm::mat4 view = m_camera.ViewMatrix();

		/*********************************************************REFLECTION***************************************************/
		m_fbos.BindReflection();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.3f, 0.2f, 1.0f, 1.0f);

		m_terrainShader.UseProgram();
		m_terrainShader.UniformData(&m_projectionMatrix[0][0], &view[0][0]);

		VAO* terrainvao = m_terrain.Vao();
		Buffer* terrainbuffer = m_terrain.Buff();
		terrainvao->Bind();
		terrainbuffer->Bind(GL_ELEMENT_ARRAY_BUFFER);
		glDrawElements(GL_TRIANGLES, m_terrain.IndexCount(), GL_UNSIGNED_SHORT, m_terrain.Offset());

		// render water
		m_waterShader.UseProgram();
		m_waterShader.UniformData(&m_projectionMatrix[0][0], &view[0][0]);

		VAO* waterVAO = m_water.Vao();
		Buffer* waterBuffer = m_water.Buff();

		waterVAO->Bind();
		waterBuffer->Bind(GL_ELEMENT_ARRAY_BUFFER);
		glDrawElements(GL_TRIANGLES, m_water.Count(), GL_UNSIGNED_SHORT, m_water.Offset());

		/********************************************************DEFAULT FBO************************************************/
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, 1700, 1200);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.3f, 0.2f, 1.0f, 1.0f);
		m_terrainShader.UseProgram();
		m_terrainShader.UniformData(&m_projectionMatrix[0][0], &view[0][0]);

		terrainvao->Bind();
		terrainbuffer->Bind(GL_ELEMENT_ARRAY_BUFFER);
		glDrawElements(GL_TRIANGLES, m_terrain.IndexCount(), GL_UNSIGNED_SHORT, m_terrain.Offset());

		// render water
		m_waterShader.UseProgram();
		m_waterShader.UniformData(&m_projectionMatrix[0][0], &view[0][0]);

		waterVAO->Bind();
		waterBuffer->Bind(GL_ELEMENT_ARRAY_BUFFER);
		glDrawElements(GL_TRIANGLES, m_water.Count(), GL_UNSIGNED_SHORT, m_water.Offset());
		
		
		/********************************************************GUI************************************************/
		m_guiShader.UseProgram();
		glBindTexture(GL_TEXTURE0, m_fbos.TextureIDReflection());
		m_guiShader.UniformData();
		
		VAO* guiVAO = m_water.Vao();
		Buffer* guiBuffer = m_water.Buff();
		guiVAO->Bind();
		guiBuffer->Bind(GL_ELEMENT_ARRAY_BUFFER);
		glDrawElements(GL_TRIANGLES, m_gui.Count(), GL_UNSIGNED_SHORT, m_gui.Offset());
	}
	Camera* Cam(void) { return &m_camera; }
private:
	glm::mat4 m_projectionMatrix;
	SHProgram m_guiShader;
	SHProgram m_waterShader;
	SHProgram m_terrainShader;

	WaterFramebuffers m_fbos;
	WaterQuad m_water;
	Terrain m_terrain;
	Camera m_camera;
	GUI m_gui;
};