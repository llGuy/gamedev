#ifndef	FRAMEBUFFER_HEADER
#define FRAMEBUFFER_HEADER

#include <GL/glew.h>
#include <stdint.h>

class Framebuffers
{
private:
	static constexpr int REFLECTIONWIDTH = 320;
	static constexpr int REFRACTIONWIDTH = 1280;

	static constexpr int REFLECTIONHEIGHT = 180;
	static constexpr int REFRACTIONHEIGHT = 720;
public:
	Framebuffers(void) = default;
	void Init(void) 
	{
		m_refractionframeBufferID = CreateFrameBuffer();
		m_refractiontextureID = CreateTextureAttachment(REFRACTIONWIDTH, REFRACTIONHEIGHT);
		m_refractiondepthTextureID = CreateDepthTextureAttachment(REFRACTIONWIDTH, REFRACTIONHEIGHT);

		m_reflectionframeBufferID = CreateFrameBuffer();
		m_reflectiontextureID = CreateTextureAttachment(REFLECTIONWIDTH, REFLECTIONHEIGHT);
		m_reflectiondepthBufferID = CreateDepthBufferAttachment(REFLECTIONWIDTH, REFLECTIONHEIGHT);
	}

	// binding fbos
	void BindFrameBuffer(uint32_t framebuffer, int width, int height, uint32_t texture)
	{
		glBindTexture(GL_TEXTURE_2D, texture);		// to make sure that the texture is not bound
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glViewport(0, 0, width, height);
	}

	void Unbind(void)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, 1200, 700);
	}
	void BindReflection(void)
	{
		BindFrameBuffer(m_reflectionframeBufferID, REFLECTIONWIDTH, REFLECTIONHEIGHT, m_reflectiontextureID);
	}
	void BindRefraction(void)
	{
		BindFrameBuffer(m_refractionframeBufferID, REFRACTIONWIDTH, REFRACTIONHEIGHT, m_refractiontextureID);
	}
	uint32_t TextureIDReflection(void)
	{
		return m_reflectiontextureID;
	}
	uint32_t TextureIDRefraction(void)
	{
		return m_refractiontextureID;
	}
private:
	uint32_t CreateFrameBuffer(void)
	{
		uint32_t frameBuffer;
		// create framebuffer
		glGenFramebuffers(1, &frameBuffer);
		// bind framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
		// which color attachment you want to bind to fbo
		// you can bind multiple if you want
		glDrawBuffer(GL_COLOR_ATTACHMENT0);

		return frameBuffer;
	}
	uint32_t CreateTextureAttachment(int width, int height)
	{
		uint32_t texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture, 0);

		return texture;
	}
	uint32_t CreateDepthTextureAttachment(int width, int height)
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
	// add a depth buffer attachment that ISNT a texture
	uint32_t CreateDepthBufferAttachment(int width, int height)
	{
		uint32_t depthBuffer;
		// non texture attachment = renderbuffers
		glGenRenderbuffers(1, &depthBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_COMPONENT, GL_RENDERBUFFER, depthBuffer);

		return depthBuffer;
	}
private:
	uint32_t m_reflectionframeBufferID;
	uint32_t m_reflectiontextureID;
	uint32_t m_reflectiondepthBufferID;


	uint32_t m_refractionframeBufferID;
	uint32_t m_refractiontextureID;
	uint32_t m_refractiondepthTextureID;
};

#endif