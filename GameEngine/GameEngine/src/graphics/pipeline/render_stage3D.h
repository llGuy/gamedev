#pragma once

#include "render_stage.h"
#include "../../api/api.h"
#include "../2D/gui/gui_handler.h"
#include "../3D/material/material_handler.h"

class render_stage3D : public render_stage
{
private:

	material_handler * materials;

	camera * scene_camera;
	
	timer * timeh;

public:

	render_stage3D(material_handler * materials, camera * scene_camera, timer & timeh);

	auto init(u32 w, u32 h) -> void override;

	auto execute(u32 current_id, std::vector<render_stage *> & stages) -> void override;

};