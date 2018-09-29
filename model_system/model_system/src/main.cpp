#include "inc/types.h"

#include "model_sys/component_system.h"

auto main(i32 argc, char * argv[]) -> i32
{
	struct some_data
	{
		int x;
	};

	using some_obj = object<some_data>;

	component_system<some_data> csys;

	vec_dd<some_obj> objs;

	some_obj new_obj;

	objs.add(new_obj);

	csys.add_system<int>(5);

	csys.add_component<int>(objs[0], 0, 42);

	csys.update(0.0f, objs);

	return 0;
}