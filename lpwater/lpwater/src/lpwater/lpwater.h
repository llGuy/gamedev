#ifndef LPWATER_HEADER
#define LPWATER_HEADER

#include "../buffer/buffer.h"
#include "../data/index_data.h"
#include "../data/vertex_data.h"
#include "../shader/shprogram.h"
#include "../primitives/vertex.h"

class LPWater
{
public:
	struct WaterDimensions
	{
		float x;
		float z;
		float yMax;
	};
	struct Quad
	{
		VertexData vData;
		IndexData iData;
	};

	explicit LPWater(WaterDimensions dim)
		: m_buffer(),
		m_shprogram("res\\vsh.shader", "res\\fsh.shader", "res\\gsh.shader"),
		m_dimensions(dim)
	{
		GenerateVertices();
		GenerateIndices();
		m_shprogram.Compile();
		m_shprogram.Link();
		GetUniformLocations();
		m_buffer.Init(m_quad.vData, m_quad.iData);

		CreateFrameBufferObject();
		CreateTextureAttachment(80, 80);
		CreateDepthTextureAttachment(80, 80);
		CreateDepthBufferAttachment(80, 80);
	}
	~LPWater(void)
	{
	}
	void Draw(glm::mat4 proj, glm::mat4 view, int width, int height)
	{
		BindFramebuffer(80, 80);

		m_buffer.BindAll();
		glClearColor(0.8f, 0.3f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glm::mat4 model = glm::mat4(1.0f);
		SendUniformData(proj, view, model);
		glDrawElements(GL_TRIANGLES, m_buffer.NumIndices(), GL_UNSIGNED_SHORT, 0);

		UnbindFramebuffer(width, height);

		glDrawElements(GL_TRIANGLES, m_buffer.NumIndices(), GL_UNSIGNED_SHORT, 0);
	}
private: 
	void GenerateVertices(void)
	{
		m_quad.vData.vData = new Vertex[4];
		m_quad.vData.vData[0].pos = glm::vec3(-m_dimensions.x / 2, 0.0f, -m_dimensions.z / 2);
		m_quad.vData.vData[1].pos = glm::vec3(+m_dimensions.x / 2, 0.0f, -m_dimensions.z / 2);
		m_quad.vData.vData[2].pos = glm::vec3(+m_dimensions.x / 2, 0.0f, +m_dimensions.z / 2);
		m_quad.vData.vData[3].pos = glm::vec3(-m_dimensions.x / 2, 0.0f, +m_dimensions.z / 2);

		m_quad.vData.vData[0].color = glm::vec3(0.0f, 0.0f, 1.0f);
		m_quad.vData.vData[1].color = glm::vec3(0.0f, 0.0f, 1.0f);
		m_quad.vData.vData[2].color = glm::vec3(0.0f, 0.0f, 1.0f);
		m_quad.vData.vData[3].color = glm::vec3(0.0f, 0.0f, 1.0f);

		m_quad.vData.numVertices = 4;
	}
	void GenerateIndices(void)
	{
		m_quad.iData.iData = new unsigned short[6];
		m_quad.iData.iData[0] = 0;
		m_quad.iData.iData[1] = 3;
		m_quad.iData.iData[2] = 2;
		m_quad.iData.iData[3] = 0;
		m_quad.iData.iData[4] = 2;
		m_quad.iData.iData[5] = 1;

		m_quad.iData.numIndices = 6;
	}
	void GetUniformLocations(void)
	{
		m_projLoc = glGetUniformLocation(m_shprogram.ProgramID(), "projection");
		m_viewLoc = glGetUniformLocation(m_shprogram.ProgramID(), "view");
		m_modelLoc = glGetUniformLocation(m_shprogram.ProgramID(), "model");
	}
	void SendUniformData(glm::mat4 proj, glm::mat4 view, glm::mat4 model)
	{
		glUniformMatrix4fv(m_projLoc, 1, GL_FALSE, &proj[0][0]);
		glUniformMatrix4fv(m_viewLoc, 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(m_modelLoc, 1, GL_FALSE, &model[0][0]);
	}
	void CreateFrameBufferObject(void)
	{
		glGenFramebuffers(1, &m_frameBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
		glDrawBuffer(GL_COLOR_ATTACHMENT0);
	}
	void CreateTextureAttachment(int width, int height)
	{
		glGenTextures(1, &m_texture);
		glBindTexture(GL_TEXTURE_2D, m_texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
			m_texture, 0);
	}
	void CreateDepthTextureAttachment(int width, int height)
	{
		glGenTextures(1, &m_depthTexture);
		glBindTexture(GL_TEXTURE_2D, m_depthTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_depthTexture, 0);
	}
	void CreateDepthBufferAttachment(int width, int height)
	{
		glGenRenderbuffers(1, &m_depthBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, m_depthBuffer); 
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthBuffer);
	}
	void BindFramebuffer(int width, int height)
	{
		glBindBuffer(GL_TEXTURE_2D, 0);
		glBindFramebuffer(GL_TEXTURE_2D, 0);
		glViewport(0, 0, width, height);
	}
	void UnbindFramebuffer(int width, int height)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, width, height);
	}
private:
	WaterDimensions m_dimensions;
	SHProgram m_shprogram;
	Buffer m_buffer;
	Quad m_quad;

	signed int m_projLoc;
	signed int m_viewLoc;
	signed int m_modelLoc;

	unsigned int m_frameBuffer;
	unsigned int m_depthBuffer;
	unsigned int m_texture;
	unsigned int m_depthTexture;
};

#endif