#include "Trashcan.hpp"
#include <utility>
#include <algorithm>
#include <sstream>
#include <stdexcept>

std::vector<Trashcan> Trashcan::trashcans;

/**
 * Get the trashcan responsible for forFile, for the specified user.
 */
const Trashcan& Trashcan::getTrashcan(fs::path forFile, const User& user)
{
	forFile = fs::absolute(std::move(forFile));
	if (isInHome(forFile, user))
	{
		return getTrashcanInDirectory(user.getXDG_DATA_HOME(), user, false);
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
	auto it = std::find(trashcans.begin(), trashcans.end(), directory_path);
	if (it == trashcans.end())
	{
		it = trashcans.begin();
		it = trashcans.insert(it, Trashcan(directory_path, user, fs_trash));
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
	:directory{path},
	 uid{user.getUID()},
	 fs_trash{fs_trash}
{
	if (fs_trash == false)
	{
		this->path = path / "Trash";
	}
	else
	{
		setTrashcanInTopDirectory(std::move(path));
	}

	if (fs::is_directory(this->path) == false)
	{
		std::ostringstream oss;
		oss << "Trash " << this->path << " is not a directory";
		throw std::runtime_error{oss.str()};
	}
}

void Trashcan::setTrashcanInTopDirectory(const fs::path& path, bool dotTrash)
{
	boost::system::error_code ec;
	std::ostringstream oss;

	if (dotTrash)
	{
		this->path = path / ".Trash";
		if (fs::exists(this->path) == false)
		{
			setTrashcanInTopDirectory(path, false);
		}
	}
	else
	{
		oss << ".Trash-" << this->uid;
		this->path = path / oss.str();
		this->createDirectory();
	}
}

void Trashcan::createDirectory()
{
	boost::system::error_code ec;
	if (fs::exists(this->path) == false)
	{
		fs::create_directories(this->path, ec);
		if (!!ec)
		{
			std::ostringstream oss;
			oss << "Can't create directory " << this->path;
			throw std::runtime_error{oss.str()};
		}
	}
}

bool Trashcan::operator==(const fs::path& directory_path)
{
	return this->directory == path;
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

