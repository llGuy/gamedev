#ifndef VAO_HEADER
#define VAO_HEADER

#include "../chunk/gpu/gpublockdata.h"

class VAO
{
public:
	virtual ~VAO(void)
	{
	}
	virtual void Init(void* d) = 0;
	virtual void Bind(void) const = 0;
	virtual void UnBind(void) const = 0;
protected:
	uint32_t m_vaoID;
};

#endif