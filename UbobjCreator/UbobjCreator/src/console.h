#pragma once

#include <iostream>

enum windows_api_color : int
{
	blue = 1,
	green = 2,
	aqua = 3,
	red = 4,
	purple = 5,
	yellow = 6,
	white = 7,
	gray = 8,
	light_blue = 9
};

typedef void * HANDLE;

class console
{
private:
	HANDLE console_handle;
public:
	static auto get(void)->console *;

	static auto set_color(HANDLE console, windows_api_color color) -> void;

	template <typename ... T> auto output_info(T ... infos) -> void
	{
		set_color(console_handle, light_blue);
		std::cout << "[INFO-" << __TIME__ << "] ";
		set_color(console_handle, white);
		(std::cout << ... << infos) << std::endl;
	}

	template <typename ... T> auto output_warning(T ... warnings) -> void
	{
		set_color(console_handle, yellow);
		std::cout << "[WARN-" << __TIME__ << "] ";
		set_color(console_handle, white);
		(std::cout << ... << warnings) << std::endl;
	}

	template <typename ... T> auto output_error(T ... errors) -> void
	{
		set_color(console_handle, red);
		std::cout << "[ERROR-" << __TIME__ << "] ";
		set_color(console_handle, white);
		(std::cout << ... << errors) << std::endl;
	}

	auto mark(char const * m) -> void
	{
		set_color(console_handle, green);
		std::cout << "======================= " << m << " =======================" << std::endl;
		set_color(console_handle, white);
	}
private:
	console(HANDLE handle);
};

#define LOG_INFO(...) console::get()->output_info(__VA_ARGS__);
#define LOG_WARN(...) console::get()->output_warning(__VA_ARGS__);
#define LOG_ERROR(...) console::get()->output_error(__VA_ARGS__);
#define LOG_MARK(txt) console::get()->mark(txt);