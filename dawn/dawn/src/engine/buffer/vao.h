#ifndef VAO_HEADER
#define VAO_HEADER
#include <GL/glew.h>
#include <stdint.h>

namespace dawn {

	class VAO
	{
	public:
		VAO(void) = default;
		~VAO(void)
		{
			glDeleteVertexArrays(1, &m_vaoID);
		}
		void Init(void)
		{
			glGenVertexArrays(1, &m_vaoID);
			Bind();
		}
		void EnableArray(const uint32_t& loc)
		{
			glEnableVertexAttribArray(loc);
		}
		void VertexAttribPtr(const uint32_t& index, const uint32_t& size, const GLenum& type, const uint32_t& stride, void* ptr)
		{
			glVertexAttribPointer(index, size, type, /*never normalized*/ GL_FALSE, stride, ptr);
		}
		void Divisor(const uint32_t& index, const uint32_t& divisor)
		{
			glVertexAttribDivisor(index, divisor);
		}
		__forceinline
		void Bind(void) const
		{
			glBindVertexArray(m_vaoID);
		}
		__forceinline
		void UnBind(void) const
		{
			glBindVertexArray(0);
		}
		__forceinline
		const uint32_t& VAOID(void) const
		{
			return m_vaoID;
		}
	protected:
		uint32_t m_vaoID;
	};

}

#endif