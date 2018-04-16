#ifndef GUIVAO_HEADER
#define GUIVAO_HEADER

#include "../../buffer/vao.h"
#include "../tpos/texture_position.h"

namespace minecraft
{
	namespace gui
	{
		class GUIVAO
			: public VAO
		{
		public:
			explicit GUIVAO(void) = default;
			void Init(void* d) override;
			void Bind(void) const override;
			void UnBind(void) const override;
			void Destroy(void) const override;
		};
	}
}

#endif