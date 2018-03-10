#include "blockselector.h"

namespace minecraft
{
	namespace gui
	{
		BlockSelector::BlockSelector(const position_t& position, const float& stride, 
			const glm::vec2& p, const float scale, TextureAtlas& t, TextureAtlas* blockt)
			: m_stride(stride), GUI::GUI(6, p, scale, new GUIVAO, t), m_position(position), m_slot(0)
		{
			m_slots[0].type = Block::block_t::GRASS;			
			m_slots[1].type = Block::block_t::OAK_LOG;
			m_slots[2].type = Block::block_t::COBBLE;
			m_slots[3].type = Block::block_t::OAK_PLANKS;
			m_slots[4].type = Block::block_t::LEAVES;
			m_slots[5].type = Block::block_t::SAND;
			m_slots[6].type = Block::block_t::BEDROCK;
			m_slots[7].type = Block::block_t::DIRT;
			m_slots[8].type = Block::block_t::GLASS;

			for(uint32_t i = 0; i < 9; ++i)
				m_slots[i].slotgui = new SlotGUI(position_t::BOTTOM, stride, glm::vec2(0.0f), 0.06f, blockt, i, m_slots[i].type);
		}
		void BlockSelector::Update(const int32_t& slot)
		{
			m_slot = slot;
			Quad newQuad = m_selector;
			for (auto& i : newQuad.cs)
			{
				i.p.x += slot * m_stride;
			}
			Quad newquads[1] = { newQuad };
			std::array<uint16_t, 6> indexbones = { 0, 1, 2, 0, 2, 3 };
			m_buffer.Update(newquads, 1, indexbones);
			BlockSelectorRndDataInit();
		}

		void BlockSelector::Init(const glm::mat4& projection)
		{
			float xrad = 1.0f / 18.0f;
			float yrad = 0.06f;
			float offsety = m_position * 0.8f;
			float offsetx = -(3.35f * (1.0f / 9.0f));

			glm::vec4 nn = projection * glm::vec4(-xrad, -yrad, 0.0f, 0.0f);
			m_selector.cs[0].p = glm::vec2(nn.x + offsetx, nn.y + offsety);
			glm::vec4 np = projection * glm::vec4(-xrad, +yrad, 0.0f, 0.0f);
			m_selector.cs[1].p = glm::vec2(np.x + offsetx, np.y + offsety);
			glm::vec4 pp = projection * glm::vec4(xrad, yrad, 0.0f, 0.0f);
			m_selector.cs[2].p = glm::vec2(pp.x + offsetx, pp.y + offsety);
			glm::vec4 pn = projection * glm::vec4(xrad, -yrad, 0.0f, 0.0f);
			m_selector.cs[3].p = glm::vec2(pn.x + offsetx, pn.y + offsety);

			m_translation = nn;

			m_selector.cs[0].t = glm::vec2(+0.0f, +1.2f / 7.0f);
			m_selector.cs[2].t = glm::vec2(+1.2f / 14.0f, +1.2f / 14.0f);
			m_selector.cs[1].t = glm::vec2(+0.0f, +1.2f / 14.0f);
			m_selector.cs[3].t = glm::vec2(+1.2f / 14.0f, +1.2f / 7.0f);

			Quad quads[1] = {m_selector};
			std::array<uint16_t, 6> indexbones = { 0, 1, 2, 0, 2, 3 };
			m_buffer.Init(quads, 1, indexbones);
			m_vao->Init(m_selector.cs);
			//m_textureAtlas->Init();

			for (uint32_t i = 0; i < 9; ++i)
			{
				m_slots[i].slotgui->Init(projection);
			}
			RenderDataInit();
		}
		void* BlockSelector::IndexOffset(void)
		{
			return (void*)(sizeof(Quad));
		}
		const Block::block_t BlockSelector::SelectedBlock(void)
		{
			return m_slots[m_slot].type;
		}
		void BlockSelector::BlockSelectorRndDataInit(void)
		{
			rnd::GLDrawElementsRenderData d;
			d.bufferData = m_buffer.BufferID();
			d.offsetIndices = IndexOffset();
			d.vao = m_vao;
			d.t = m_textureAtlas->Tex();
			d.count = Count();
			m_rndData[0] = d;
		}
		void BlockSelector::SlotGUIRndDataInit(void)
		{
			for (uint32_t i = 0; i < 9; ++i)
			{
				rnd::GLDrawElementsRenderData d;
				d.bufferData = m_slots[i].slotgui->BufferID();
				d.offsetIndices = m_slots[i].slotgui->IndexOffset();
				d.vao = m_slots[i].slotgui->Vao();
				d.t = m_slots[i].slotgui->Tex()->Tex();
				d.count = m_slots[i].slotgui->Count();
				m_rndData[i + 1] = d;
			}
		}
		void BlockSelector::RenderDataInit(void)
		{
			BlockSelectorRndDataInit();
			SlotGUIRndDataInit();
		}
		rnd::GLDrawElementsRenderDataAll BlockSelector::DrawData(void)
		{
			return {m_rndData, 10};
		}
	}
}