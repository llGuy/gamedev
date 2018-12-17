#include <windows.h>
#include "console.h"

auto console::get(void) -> console *
{
	static console * global_console = nullptr;

	if (!global_console)
	{
		global_console = new console(GetStdHandle(STD_OUTPUT_HANDLE));
	}

	return global_console;
}

auto console::set_color(HANDLE console, windows_api_color color) -> void
{
	SetConsoleTextAttribute(console, color);
}

console::console(HANDLE handle)
	: console_handle(handle)
{
}