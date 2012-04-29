#include "Trashcan.hpp"
#include <algorithm>

std::vector<Trashcan> Trashcan::trashcans;

/**
 * Get the trashcan responsible for forFile, for the specified user.
 */
const Trashcan& Trashcan::getTrashcan(fs::path forFile, const User& user)
{
	forFile = fs::absolute(forFile);
	if (isInHome(forFile, user))
	{
		return getTrashcanInDirectory(user.getHome(), user, false);
	}
	return getTrashcanInDirectory(cppmounts::for_path(forFile.string())->getPath(), user);
}

bool Trashcan::isInHome(const fs::path& file, const User& user)
{
	fs::path home{user.getHome()};
	for (auto it = home.begin(), file_it = file.begin();
		 it != home.end() && file_it != file.end();
		 it++, file_it++)
	{
		if (*it != *file_it)
		{
			return false;
		}
	}
	return true;
}

Trashcan& Trashcan::getTrashcanInDirectory(fs::path directory_path, const User& user, bool fs_trash)
{
	fs::path path;
	if (fs_trash)
	{
		path = directory_path / ".Trash";
	}
	else
	{
		path = directory_path / "Trash";
	}

	auto it = std::find(trashcans.begin(), trashcans.end(), path);
	if (it == trashcans.end())
	{
		it = trashcans.begin();
		it = trashcans.insert(it, Trashcan(directory_path, user, fs_trash));
		std::cout << "new trashcan" << std::endl;
	}
	return *it;
}

/**
 * Initialize a Trashcan in directory pointed by path.
 * The constructor checks the Trash as described by freedesktop.org
 * trash can specifications.
 *
 * The optional parameter fs_trash control whether the trash directory
 * should be hidden or not.
 */
Trashcan::Trashcan(fs::path path, const User& user, bool fs_trash)
	:uid{user.getUID()},
	 fs_trash{fs_trash}
{
	if (fs_trash)
	{
		this->path = path / ".Trash";
	}
	else
	{
		this->path = path / "Trash";
	}
}

bool Trashcan::operator==(const fs::path& path)
{
	return this->path == path;
}

const fs::path& Trashcan::getPath() const noexcept
{
	return this->path;
}

Trashcan::operator const fs::path&()
{
	return this->path;
}

Trashcan::operator const std::string&()
{
	return this->path.string();
}

