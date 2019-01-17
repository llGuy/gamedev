#include "render_stage3D.h"

render_stage3D::render_stage3D(material_handler * materials, camera * scene_camera, timer & timeh)
	: materials(materials), scene_camera(scene_camera), timeh(&timeh)
{
}

auto render_stage3D::init(u32 w, u32 h) -> void
{
	fbo.create(w, h);
}

auto render_stage3D::execute(u32 current_id, std::vector<render_stage *> & stages) -> void
{
	glEnable(GL_DEPTH_TEST);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	materials->render_all(*scene_camera, *timeh);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}