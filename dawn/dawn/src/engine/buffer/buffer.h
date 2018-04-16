#ifndef BUFFER_HEADER
#define BUFFER_HEADER
#include <GL/glew.h>
#include <stdint.h>

namespace dawn {

	class Buffer
	{
	public:
		Buffer(void) = default;
		~Buffer(void)
		{
			glDeleteBuffers(1, &m_bufferID);
		}
		template<typename _Ty>
		void Init(uint32_t size, _Ty* data, GLenum usage)
		{
			glGenBuffers(1, &m_bufferID);
			Bind(GL_ARRAY_BUFFER);
			glBufferData(GL_ARRAY_BUFFER, size, data, usage);
		}
		template<typename _Ty>
		void SubData(uint32_t offset, uint32_t size, _Ty* data)
		{
			glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
		}
		__forceinline
		const uint32_t BufferID(void) const noexcept
		{
			return m_bufferID;
		}
		__forceinline
		void Bind(GLenum point) const
		{
			glBindBuffer(point, m_bufferID);
		}
	protected:
		uint32_t m_bufferID;
	};

}

#endif