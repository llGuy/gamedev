#ifndef _MSG_ENCODER_H_
#define _MSG_ENCODER_H_

#include <vector>
#include <string>

#include "msg_parser.h"


namespace mulgame {

    class MSGEncoder
    {
    public:
	MSGEncoder(void) = default;

	std::vector<Byte>& Vector(void)
        {
	    return m_bytes;
	}

	uint32_t Size(void)
	{
	    return m_bytes.size();
	}
	
	Byte* Data(void)
	{
	    return &m_bytes[0];
	}

	void PushString(const std::string& str)
	{
	    for(uint32_t i = 0; i < str.length(); ++i)
	    {
		m_bytes.push_back(str[i]);
	    }
	    m_bytes.push_back(CHAR_DELIMITER);
	}
	
	template<typename _Ty>
	void PushBytes(_Ty&& val)
	{
	    uint32_t size = sizeof(val);
	    Byte* valptrCast = (Byte*)&val;
	    for(uint32_t ptr = 0; ptr < size; ++ptr)
	    {
		m_bytes.push_back(valptrCast[ptr]);
	    }
	    m_bytes.push_back(CHAR_DELIMITER);
	}

	template<typename _First, typename... _Rest>
	void PushBytes(_First&& first, _Rest&&... rest)
	{
	    PushBytes(first);
	
	    PushBytes(std::forward<_Rest>(rest)...);
	}
    
    private:
	std::vector<Byte> m_bytes;
    };

}

#endif /* _MSG_ENCODER_H_ */
