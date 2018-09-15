#pragma once

#include <string>
#include <iostream>

namespace logger {

	template <typename T> auto print(T && item) -> void
	{
		std::cout << item;
	}
	inline auto return_line(void) -> void
	{
		std::cout << std::endl;
	}
	template <typename ... Args> auto sub_log(Args && ... args) -> void
	{
		std::cout << "log > ";
		int8_t prints[]{ 0, (print(args), static_cast<int8_t>(0))... };
		return_line();
	}
	template <typename ... Args> auto new_log(std::string const & title, Args && ... args) -> void
	{
		std::cout << "------ " << title << " ------ \n";
		int8_t prints[]{ 0, (print(args), static_cast<int8_t>(0))... };
		return_line();
	}
	template <typename ... Args> auto error_log(Args && ... args) -> void
	{
		std::cout << "error > ";
		int8_t prints[]{ 0, (print(args), static_cast<int8_t>(0))... };
		return_line();
	}

}
