#ifndef _VEC_IO_H_
#define _VEC_IO_H_

#include <iostream>
#include <glm/glm.hpp>
#include <stdint.h>

void VOut(const glm::vec3& vector)
{
    for(uint32_t i = 0; i < 3; ++i)
	std::cout << vector[i] << " ";
}

#endif /* _VEC_IO_H_ */
