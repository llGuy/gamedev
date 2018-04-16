#ifndef ERROR_HEADER
#define ERROR_HEADER

namespace dawn {namespace utils {
		
		template<typename _Ty>
		void Error(_Ty&& msg)
		{
			std::cout << "-----------  ERROR  : " << msg << "  -----------" << std::endl;
		}

	} 
}

#endif