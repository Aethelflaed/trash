#ifndef TRASH_TRASHCAN_HPP
#define TRASH_TRASHCAN_HPP

#include <boost/filesystem.hpp>
#include <vector>
#include <string>
#include "User.hpp"

namespace fs = ::boost::filesystem;

class Trashcan
{
public:
	static const Trashcan& getTrashcan(fs::path forFile, const User& user);

	const fs::path& getPath() const noexcept;

	operator const fs::path&();
	operator const std::string&();

private:
	Trashcan(fs::path path, bool fs_trash = true);

	static std::vector<Trashcan> trashcans;

	fs::path path;
};

#endif /* TRASH_TRASHCAN_HPP */

