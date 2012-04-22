#ifndef TRASH_TRASH_HPP
#define TRASH_TRASH_HPP

#include "Options.hpp"
#include <boost/filesystem.hpp>

namespace fs = ::boost::filesystem;

class Trash
{
public:
	Trash(int argc, const char** argv) noexcept;

	int run();

private:
	Options options{};
	std::vector<fs::path> paths{};
};

#endif /* TRASH_TRASH_HPP */

