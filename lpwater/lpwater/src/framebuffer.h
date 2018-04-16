#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>

class WaterFramebuffers
{
	static constexpr int REFLECTIONWIDTH = 320;
	static constexpr int REFRACTIONWIDTH = 1280;

	static constexpr int REFLECTIONHEIGHT = 180;
	static constexpr int REFRACTIONHEIGHT = 720;
public:
	WaterFramebuffers(int32_t width, int32_t height)
		: m_displayWidth(width), m_displayHeight(height)
	{
		m_refractionFBO = CreateFBO();
		m_refractionTex = CreateTexAttachment(REFRACTIONWIDTH, REFRACTIONHEIGHT);
		m_refractionDepthTex = CreateDepthTexAttachment(REFRACTIONWIDTH, REFRACTIONHEIGHT);

		m_reflectionFBO = CreateFBO();
		m_reflectionTex = CreateTexAttachment(REFLECTIONWIDTH, REFLECTIONHEIGHT);
		m_reflectionDepthBuf = CreateDepthBufferAttachment(REFLECTIONWIDTH, REFLECTIONHEIGHT);
	}
	void BindFramebuffer(uint32_t framebuffer, int32_t width, int32_t height, uint32_t texture)
	{
		glBindTexture(GL_TEXTURE_2D, texture);		// to make sure that the texture is not bound
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glViewport(0, 0, width, height);
	}
	void BindReflection(void)
	{
		BindFramebuffer(m_reflectionFBO, REFLECTIONWIDTH, REFLECTIONHEIGHT, m_reflectionTex);
	}
	void BindRefraction(void)
	{
		BindFramebuffer(m_refractionFBO, REFRACTIONWIDTH, REFRACTIONHEIGHT, m_refractionTex);
	}
	uint32_t TextureIDReflection(void)
	{
		return m_reflectionTex;
	}
	uint32_t TextureIDRefraction(void)
	{
		return m_refractionTex;
	}
private:
	uint32_t CreateFBO(void)
	{
		uint32_t fbo;
		glGenFramebuffers(1, &fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glDrawBuffer(GL_COLOR_ATTACHMENT0);
		return fbo;
	}
	uint32_t CreateTexAttachment(int32_t width, int32_t height)
	{
		uint32_t tex;
		glGenTextures(1, &tex);
		glBindTexture(GL_TEXTURE_2D, tex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, tex, 0);
		
		return tex;
	}
	uint32_t CreateDepthTexAttachment(int32_t width, int32_t height)
	{
		uint32_t depthTextureID;
		glGenTextures(1, &depthTextureID);
		glBindTexture(GL_TEXTURE_2D, depthTextureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTextureID, 0);

		return depthTextureID;
	}
	uint32_t CreateDepthBufferAttachment(int32_t width, int32_t height)
	{
		uint32_t depthBuffer;
		glGenRenderbuffers(1, &depthBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_COMPONENT, GL_RENDERBUFFER, depthBuffer);

		return depthBuffer;
	}
private:
	int32_t m_displayWidth;
	int32_t m_displayHeight;

	uint32_t m_refractionFBO;
	uint32_t m_refractionTex;
	uint32_t m_refractionDepthTex;

	uint32_t m_reflectionFBO;
	uint32_t m_reflectionTex;
	uint32_t m_reflectionDepthBuf;
};