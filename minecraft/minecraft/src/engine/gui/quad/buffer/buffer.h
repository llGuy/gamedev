#ifndef BUFFER_HEADER
#define BUFFER_HEADER

#include <GL/glew.h>

#include "../quad.h"

namespace minecraft
{
	namespace gui
	{
		class GUIBuffer
		{
		public:
			explicit GUIBuffer(void);
			void Init(const Quad& q);
			void Bind(void);
			void UnBind(void);
			uint32_t BufferID(void);
		private:
			uint32_t m_bufferID;
			uint32_t m_indexBufferID;
		};
	}
}

#endif