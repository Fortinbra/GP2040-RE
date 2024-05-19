#pragma once

enum class Console
{
	CONSOLE_UNKNOWN,
	CONSOLE_NES,
	CONSOLE_SNES,
	CONSOLE_N64,
	CONSOLE_GC,
	CONSOLE_PS1,
	CONSOLE_PS2,
	CONSOLE_DC
};
struct ConsoleLookup
{
	Console console;
	double frequency;
};

inline ConsoleLookup consoleLookupTable[] = {
	{Console::CONSOLE_NES, 1789.773},
	{Console::CONSOLE_SNES, 1789.773},
	{Console::CONSOLE_N64, 3.2},
	{Console::CONSOLE_GC, 1490},
	{Console::CONSOLE_PS1, 250},
	{Console::CONSOLE_PS2, 250},
	{Console::CONSOLE_DC, 250},
};
