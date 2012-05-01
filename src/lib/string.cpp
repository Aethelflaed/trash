#include "string.hpp"

std::string operator "" _s(const char* str, size_t length)
{
	return std::string(str, length);
}
