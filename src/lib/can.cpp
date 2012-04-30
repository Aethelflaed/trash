#include "can.hpp"
#include <utility>
#include <algorithm>
#include <sstream>
#include <stdexcept>

using namespace ::trash;

std::vector<can> can::trashcans;

/**
 * Get the trashcan responsible for forFile
 */
can& can::get_for(fs::path forFile)
{
	forFile = fs::absolute(std::move(forFile));
	if (isInHome(forFile))
	{
		return getCanInDirectory(user::current().get_HOME(), false);
	}
	return getCanInDirectory(cppmounts::for_path(forFile.string())->getPath());
}

bool can::isInHome(const fs::path& file)
{
	fs::path home{user::current().get_HOME()};
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

can& can::getCanInDirectory(fs::path directory_path, bool fs_trash)
{
	auto it = std::find(trashcans.begin(), trashcans.end(), directory_path);
	if (it == trashcans.end())
	{
		it = trashcans.begin();
		it = trashcans.insert(it, can(directory_path, fs_trash));
	}
	return *it;
}

/**
 * Initialize a can in directory pointed by path.
 * The constructor checks the Trash as described by freedesktop.org
 * trash can specifications.
 *
 * The optional parameter fs_trash control whether the trash directory
 * should be hidden or not.
 */
can::can(fs::path path, bool fs_trash)
	:directory{path},
	 fs_trash{fs_trash}
{
	if (fs_trash == false)
	{
		this->path = fs::path{user::current().get_XDG_DATA_HOME()} / "Trash";
		this->createDirectory();
	}
	else
	{
		setCanInTopDirectory(std::move(path));
	}

	createDirectory(this->path / "files");
	createDirectory(this->path / "info");
}

void can::setCanInTopDirectory(const fs::path& path, bool dotTrash)
{
	boost::system::error_code ec;
	std::ostringstream oss;

	if (dotTrash)
	{
		this->path = path / ".Trash";
		if (fs::exists(this->path) == false)
		{
			setCanInTopDirectory(path, false);
		}
		else
		{
			//TODO check sticky bit and so on
		}
	}
	else
	{
		oss << ".Trash-" << user::current().get_uid();
		this->path = path / oss.str();
		this->createDirectory();
	}
}

void can::createDirectory()
{
	createDirectory(this->path);
}
void can::createDirectory(const fs::path& path)
{
	boost::system::error_code ec;
	if (fs::exists(path) == false)
	{
		fs::create_directories(path, ec);
		if (!!ec)
		{
			std::ostringstream oss;
			oss << "Cannot create directory " << path;
			throw std::runtime_error{oss.str()};
		}
	}
	if (fs::is_directory(path) == false)
	{
		std::ostringstream oss;
		oss << "File " << path << " is not a directory";
		throw std::runtime_error{oss.str()};
	}
}

void can::put(const fs::path& path)
{
	//TODO info file
	//then move
}

bool can::operator==(const fs::path& directory_path)
{
	return this->directory == path;
}

const fs::path& can::getPath() const noexcept
{
	return this->path;
}

can::operator const fs::path&()
{
	return this->path;
}

can::operator const std::string&()
{
	return this->path.string();
}

