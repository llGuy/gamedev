#include "slotgui.h"

namespace minecraft
{
	namespace gui
	{
		SlotGUI::SlotGUI(const position_t& p, const float& stride, const glm::vec2 translation,
			const float s, TextureAtlas* t, uint32_t slot, Block::block_t b)
			: GUI::GUI(6 * 3, translation, s, new GUIVAO, *t), m_position(p), m_slot(slot), m_stride(stride), m_btype(b)
		{
		}
		void SlotGUI::Init(const glm::mat4& projection)
		{
			float offsety = m_position * 0.8f;
			float offsetx = -(3.35f * (1.0f / 9.0f)) + static_cast<uint32_t>(m_slot) * m_stride;

			CreateBlockImage();
			for (uint32_t q = 0; q < 3; ++q)
			{
				for (uint32_t p = 0; p < 4; ++p)
				{
					glm::vec4 pos = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
					pos.x = m_faces[q].cs[p].p.x;
					pos.y = m_faces[q].cs[p].p.y;
					m_faces[q].cs[p].p = projection * pos;
					m_faces[q].cs[p].p += glm::vec2(offsetx, offsety);
				} 
			}
			std::array<uint16_t, 6> indexbones = { 0, 1, 2, 1, 2, 3 };
			m_buffer.Init(m_faces, 3, indexbones);
			m_vao->Init(m_faces[0].cs);
		}
		void* SlotGUI::IndexOffset(void)
		{
			return (void*)(sizeof(Quad) * 3);
		}
		void SlotGUI::RotateCube(glm::vec4* v, uint32_t size, glm::mat4& rotation)
		{
			for (uint32_t i = 0; i < size; ++i)
			{
				v[i] *= m_scale;
				v[i] = rotation * v[i];
			}
		}
		void SlotGUI::CreateTopFace(QuadV4* q, const glm::vec4* v, TextureAtlasTile& tattop)
		{
			q[0].cs[0].p = v[1];						q[0].cs[0].t = tattop.tx00;
			q[0].cs[1].p = v[3];						q[0].cs[1].t = tattop.tx01;
			q[0].cs[2].p = v[5];						q[0].cs[2].t = tattop.tx10;
			q[0].cs[3].p = v[7];						q[0].cs[3].t = tattop.tx11;
		}
		void SlotGUI::CreateFrontFace(QuadV4* q, const glm::vec4* v, TextureAtlasTile& tatsides)
		{
			for (uint32_t i = 4; i < 8; ++i)
				q[1].cs[i - 4].p = v[i];

			q[1].cs[0].t = tatsides.tx10;
			q[1].cs[1].t = tatsides.tx00;
			q[1].cs[2].t = tatsides.tx11;
			q[1].cs[3].t = tatsides.tx01;
		}
		void SlotGUI::CreatePosXFace(QuadV4* q, const glm::vec4* v, TextureAtlasTile& tatsides)
		{
			q[2].cs[0].p = v[2];						q[2].cs[0].t = tatsides.tx11;
			q[2].cs[1].p = v[3];						q[2].cs[1].t = tatsides.tx01;
			q[2].cs[2].p = v[6];						q[2].cs[2].t = tatsides.tx10;
			q[2].cs[3].p = v[7];						q[2].cs[3].t = tatsides.tx00;
		}
		void SlotGUI::CreateBlockImage(void)
		{
			glm::mat4 rotationMatrix = glm::rotate(glm::radians(-50.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			rotationMatrix *= glm::rotate(glm::radians(20.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			rotationMatrix *= glm::rotate(glm::radians(-20.0f), glm::vec3(0.0f, 0.0f, 1.0f));

			QuadV4 quads[3];
			glm::vec3 faceIndices = Block::BLOCK_TEXTURE_DATA[static_cast<uint32_t>(m_btype)].topSidesBottom;
			TextureAtlasTile tattop = m_textureAtlas->Tile(faceIndices[0]);
			TextureAtlasTile tatsides = m_textureAtlas->Tile(faceIndices[1]);
			glm::vec4 verts[8]
			{
				glm::vec4(-0.5f, -0.5f, -0.5f, 1.0f),	// negative x and z
				glm::vec4(-0.5f, +0.5f, -0.5f, 1.0f),
				
				glm::vec4(+0.5f, -0.5f, -0.5f, 1.0f),	// positive x and negative z
				glm::vec4(+0.5f, +0.5f, -0.5f, 1.0f),
				
				glm::vec4(-0.5f, -0.5f, +0.5f, 1.0f),	
				glm::vec4(-0.5f, +0.5f, +0.5f, 1.0f),
				
				glm::vec4(+0.5f, -0.5f, +0.5f, 1.0f),
				glm::vec4(+0.5f, +0.5f, +0.5f, 1.0f),
			};
			RotateCube(verts, 8, rotationMatrix);
			// calculating texture coordinates and coordinates of the blocks
			// top face
			CreateTopFace(quads, verts, tattop);
			CreateFrontFace(quads, verts, tatsides);
			CreatePosXFace(quads, verts, tatsides);
			
			for (uint32_t q = 0; q < 3; ++q)
			{
				for (uint32_t p = 0; p < 4; ++p)
				{
					m_faces[q].cs[p].p = glm::vec2(quads[q].cs[p].p);
					m_faces[q].cs[p].t = quads[q].cs[p].t;
				}
			}
		}
	}
}