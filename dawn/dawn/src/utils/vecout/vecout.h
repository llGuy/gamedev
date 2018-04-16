#ifndef VECIO_HEADER
#define VECIO_HEADER

#include <iostream>
#include <glm/glm.hpp>

namespace dawn { namespace utils {

		template<typename _Ty, uint32_t _Dm>
		void OutVec(const glm::vec<_Dm, _Ty, glm::highp>& v)
		{
			for (uint32_t i = 0; i < _Dm; ++i)
				std::cout << v[i] << " ";
		}

	} 
}

#endif