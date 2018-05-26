#ifndef _ENGINE_H_
#define _ENGINE_H_

#include <glm/glm.hpp>

#include "entities_handler.h"
#include "terrain.h"
#include "event_handler.h"
#include "program.h"
#include "configs.h"
#include "renderer.h"
#include "data.h"

namespace mulgame {
    
    class MULGEngine
    {
    public:
		MULGEngine(const MULGEngine&) = delete;
		const MULGEngine& operator=(const MULGEngine&) = delete;
		
		MULGEngine(int32_t width, int32_t height);
    public:
		void Render(void);
		void Update(void);

		inline
		MULGEventForwarder EForwarder(void)
		{
		    return MULGEventForwarder(&m_ehandler, &m_terrain);
		}
    private:
		void RenderEntities(void);
		void RenderBullets(void);
		void RenderTerrain(void);
	private:
		void Configure(void);
		void InitData(int32_t width, int32_t height);
		void InitShaders(void);
		void InitEntityShaders(void);
		void InitTerrainShaders(void);
		void InitEntities(void);
		void InitTerrain(void);
		void InitOpenglGLStates(void) const;
    private:
		Terrain m_terrain;
		Renderer m_renderer;
		MULGEngineData m_data;
		MULGEngineConfigs m_configs;
		Program<2, 4> m_entityProgram;
		Program<2, 4> m_terrainProgram;
		EntitiesHandler m_ehandler;
    };

}

#endif /* _ENGINE_H_ */
