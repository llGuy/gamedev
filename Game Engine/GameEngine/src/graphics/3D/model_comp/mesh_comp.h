#pragma once

#include "../../../utils/types.h"

#include "../model_data.h"
#include "../../../data/components.h"

/* computes the model without loading from e.g. blender */
struct model_computation
{
	virtual auto compute(object<model_data> & obj) -> void {}
};