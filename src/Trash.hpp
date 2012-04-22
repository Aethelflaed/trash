#ifndef TRASH_TRASH_HPP
#define TRASH_TRASH_HPP

#include "Options.hpp"

class Trash
{
public:
	Trash(int argc, const char** argv) noexcept;

	int run();

private:
	Options options{};
};

#endif /* TRASH_TRASH_HPP */

