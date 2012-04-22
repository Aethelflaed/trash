#ifndef TRASH_TRASH_HPP
#define TRASH_TRASH_HPP

#include "Application.hpp"

class Trash : public Application
{
public:
	Trash(int argc, const char** argv) noexcept;

	int run() override;
};

#endif /* TRASH_TRASH_HPP */

