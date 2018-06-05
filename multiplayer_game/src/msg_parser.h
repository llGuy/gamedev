#ifndef _MSG_PARSER_H_
#define _MSG_PARSER_H_

#include <string>
#include <vector>
#include <type_traits>

namespace mulgame {

    using Byte = int8_t;

    enum
    : int8_t
    {
	CHAR_DELIMITER = '#'
    };
    
    class MSGParser
    {
    public:
	MSGParser(Byte* bytes, int32_t size)
	    : m_bytes(bytes), m_index(0)
	{
	}

	bool Max(void)
	{
	    return m_index >= m_size - 1;
	}
	
	template<typename _Ty>
	_Ty ReadNext(char delimiter)
	{
	    _Ty* ptr = (_Ty*)&m_bytes[m_index];
	    m_index += sizeof(_Ty) + 1 /* delimiter of section */;
	    return *ptr;
	}
	
	void ReadNext(char delimiter, std::string& s)
	{
	    for(; m_bytes[m_index] != delimiter; ++m_index)
	    {
		s.push_back(m_bytes[m_index]);
	    }
	    ++m_index;
	}
    private:
	Byte* m_bytes;
	int32_t m_size;
	uint32_t m_index;
    };
    
}

#endif /* _MSG_PARSER_H_ */
