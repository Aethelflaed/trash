#ifndef TRASH_STRING_HPP
#define TRASH_STRING_HPP

#include <string>

std::string operator "" _s(const char* str, size_t /* length */)
{
	return std::string(str);
}

#endif /* TRASH_STRING_HPP */

