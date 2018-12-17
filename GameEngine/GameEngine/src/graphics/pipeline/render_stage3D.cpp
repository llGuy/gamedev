#include "render_stage3D.h"

render_stage3D::render_stage3D(material_handler * materials, camera * scene_camera)
	: materials(materials), scene_camera(scene_camera)
{
}

auto render_stage3D::init(u32 w, u32 h) -> void
{
	fbo.create(w, h);
}

auto render_stage3D::execute(u32 current_id, std::vector<render_stage *> & stages) -> void
{
	glEnable(GL_DEPTH_TEST);

	materials->render_all(*scene_camera);
}