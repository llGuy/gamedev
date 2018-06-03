#ifndef _MSG_PARSER_H_
#define _MSG_PARSER_H_

#include <string>
#include <vector>
#include <type_traits>

namespace mulgame {

    enum
    : int8_t
    {
	CHAR_DELIMITER = '#'
    };
    
    class MSGParser
    {
    public:
	MSGParser(const std::string& msg)
	    : m_msg(msg), m_index(0)
	{
	}

	bool Max(void)
	{
	    return m_index >= (m_msg.size() - 1);
	}
	
	template<typename _Ty>
	_Ty ReadNext(char delimiter)
	{
	    _Ty* ptr = (_Ty*)&m_msg[m_index];
	    m_index += sizeof(_Ty) + 1 /* delimiter of section */;
	    return *ptr;
	}
	
	void ReadNext(char delimiter, std::string& s)
	{
	    for(uint32_t i = m_index; m_msg[i] != delimiter; ++i) s.push_back(m_msg[i]);
	    ++m_index;
	}
    private:
	std::string m_msg;
	uint32_t m_index;
    };
    
}

#endif /* _MSG_PARSER_H_ */
