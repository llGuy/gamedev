#ifndef _MSG_PROTOTYPE_H_
#define _MSG_PROTOTYPE_H_

#include <array>
#include <vector>
#include <numeric>
#include <algorithm>

namespace mulgame {

    using Byte = int8_t;

    template<typename... _Types>
    class MSGPrototype
    {
    public:
	MSGPrototype(void)
	{
	    std::array<uint32_t, sizeof...(_Types)> sizes { sizeof(_Types)... };
	    m_bytes.resize(std::accumulate(sizes.begin(), sizes.end(), 0));
	}

	template<typename _Ty>
	void Fill(_Ty&& data, uint32_t index)
	{
	    uint32_t totalOffset = std::accumulate(m_sizes.begin(), m_sizes.begin() + index, 0);
	    void* ptr = &m_bytes[totalOffset];
	    _Ty* ptrcast = static_cast<_Ty*>(ptr);
	    *ptrcast = std::forward<_Ty>(data);
	}

	Byte* Data(void)
	{
	    return m_bytes.data();
	}

	uint32_t Size(void) const
	{
	    return m_bytes.size();
	}
    private:
	std::vector<Byte> m_bytes;
	std::array<uint32_t, sizeof...(_Types)> m_sizes;
    };

}

#endif /* _MSG_PROTOTYPE_H_ */
