#ifndef BUFFER_HEADER
#define BUFFER_HEADER

#include <GL/glew.h>
#include <array>

#include "../quad.h"

namespace minecraft
{
	namespace gui
	{
		class GUIBuffer
		{
		public:
			explicit GUIBuffer(void);
			void Init(const Quad* q, uint32_t size, std::array<uint16_t, 6> ind);
			void Update(const Quad* q, uint32_t size, std::array<uint16_t, 6> ind);
			void Bind(void);
			void UnBind(void);
			uint32_t BufferID(void);
		private:
			uint32_t m_bufferID;
		};
	}
}

#endif