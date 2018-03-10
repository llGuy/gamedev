#ifndef SLOTGUI_HEADER
#define SLOTGUI_HEADER

#include "../../../../../block/block.h"
#include "../../../../gui.h"
#include "../../../../../texture_atlas/texture_atlas.h"
#include <glm/gtx/transform.hpp>

// TESTING

namespace minecraft
{
	namespace gui
	{
		class SlotGUI
			: public GUI
		{
		public:
			explicit SlotGUI(const float& stride, const glm::vec2&,
				const glm::vec2&, TextureAtlas* t);
			void Init(const glm::mat4& projection) override;
			void* IndexOffset(void) override;
		private:
			void CreateBlockImage(void);
			void RotateCube(glm::vec4* v, uint32_t size, glm::mat4& rotation);
			void CreateTopFace(QuadV4* q, const glm::vec4* v, TextureAtlasTile& tattop);
			void CreateFrontFace(QuadV4* q, const glm::vec4* v, TextureAtlasTile& tatsides);
			void CreatePosXFace(QuadV4* q, const glm::vec4* v, TextureAtlasTile& tatsides);
		private:
			Block::block_t m_btype;
			Quad m_faces[3];
		};
	}
}

#endif