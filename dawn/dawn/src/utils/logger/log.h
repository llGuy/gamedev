#ifndef LOG_HEADER
#define LOG_HEADER
#include <iostream>
#include <string>

namespace dawn { namespace utils {
		
		template<typename _Ty>
		void NewLog(_Ty&& msg)
		{
			std::cout << "-----------  " << msg << "  -----------" << std::endl;
		}

		template<typename _Fr, typename... _Args>
		void Log(_Fr&& first, _Args&&... next)
		{
			std::cout << "> " << first << std::endl;
			Log(next...);
		}

		template<typename _Ty>
		void Log(_Ty&& t)
		{
			std::cout << "> " << t << std::endl;
		}

	} 
}

#endif