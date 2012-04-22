#include "Trash.hpp"
#include <boost/filesystem.hpp>

namespace fs = ::boost::filesystem;

Trash::Trash(int argc, const char** argv) noexcept
{
	options.store_cli(argc, argv);
	options.notify();
}

int Trash::run()
{
	return 0;
}

