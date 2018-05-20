#ifndef _DATA_H_
#define _DATA_H_

#include <glm/glm.hpp>

namespace mulgame {

    struct MULGEngineData
    {
	glm::mat4 matProjection;
	float frameTime;
    };

}

#endif /* _DATA_H_ */
