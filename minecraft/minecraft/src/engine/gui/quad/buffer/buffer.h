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
			void Init(const Quad* q, uint32_t size);
			void Update(const Quad* q, uint32_t size);
			void Bind(void);
			void UnBind(void);
			uint32_t BufferID(void);
		private:
			uint32_t m_bufferID;
		};
	}
}

#endif