#ifndef _ENGINE_H_
#define _ENGINE_H_

#include <glm/glm.hpp>

#include "mtime.h"
#include "entities_handler.h"
#include "terrain.h"
#include "event_handler.h"
#include "program.h"
#include "configs.h"
#include "renderer.h"
#include "data.h"
#include "lighting.h"
#include "lflare_handler.h"
#include "network_handler.h"

namespace mulgame {
    
    class MULGEngine
    {
    public:
	MULGEngine(const MULGEngine&) = delete;
	const MULGEngine& operator=(const MULGEngine&) = delete;
		
	MULGEngine(int32_t width, int32_t height, int8_t arg);
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
	void RenderLFlare(void);
    private:
	void Configure(void);
	void InitData(int32_t width, int32_t height);
	void InitShaders(void);
	void InitEntityShaders(void);
	void InitTerrainShaders(void);
	void InitLFlareShaders(void);
	void InitEntities(void);
	void InitNetworkHandler(void);
	void InitTerrain(void);
	void InitOpenglGLStates(void) const;
	std::string ShaderPath(const std::string & shaderName) const;
    private:
	Terrain m_terrain;
	Renderer m_renderer;
	Lighting m_lighting;
	MULGEngineData m_data;
	MULGEngineConfigs m_configs;
	Program<2, 4> m_entityProgram;
	Program<3, 4> m_terrainProgram;
	Program<2, 4> m_lflareProgram;
	EntitiesHandler m_ehandler;
	LFlareHandler m_lensFlareHandler;
	NetworkHandler m_networkHandler;
    };

}

#endif /* _ENGINE_H_ */
