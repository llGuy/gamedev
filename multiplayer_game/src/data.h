#ifndef _DATA_H_
#define _DATA_H_

#include <glm/glm.hpp>

namespace mulgame {

    struct MULGEngineData
    {
		glm::mat4 matProjection;
		glm::vec3 lightPosition;
		float frameTime;
    };

}

#endif /* _DATA_H_ */
