#include "Trash.hpp"

Trash::Trash(int argc, const char** argv) noexcept
{
	options.store_cli(argc, argv);
	options.notify();
}

int Trash::run()
{
	return 0;
}

