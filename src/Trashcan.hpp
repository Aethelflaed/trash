#ifndef TRASH_TRASHCAN_HPP
#define TRASH_TRASHCAN_HPP

#include <boost/filesystem.hpp>
#include <cppmounts.hpp>
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
	bool operator==(const fs::path& path);

private:
	static bool isInHome(const fs::path& file, const User& user);
	static Trashcan& getTrashcanInDirectory(fs::path directory_path, const User& user, bool fs_trash = true);

	Trashcan(fs::path path, const User& user, bool fs_trash = true);

	static std::vector<Trashcan> trashcans;

	fs::path path;
	uid_t uid;
	bool fs_trash;
};

#endif /* TRASH_TRASHCAN_HPP */

