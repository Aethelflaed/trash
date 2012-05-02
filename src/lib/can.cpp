#include "can.hpp"
#include "string.hpp"
#include <utility>
#include <algorithm>
#include <sstream>
#include <stdexcept>
#include "trashinfo.hpp"

using namespace ::trash;

std::vector<can> can::trashcans;

/**
 * Get the trashcan responsible for forFile
 */
can& can::get_for(fs::path file)
{
	file = fs::absolute(std::move(file));
	if (is_in_home(file))
	{
		return can_for_directory(user::current().get_HOME(), false);
	}

	std::string top_dir{cppmounts::for_path(file.string())->getPath()};

	return can_for_directory(top_dir);
}

bool can::is_in_home(const fs::path& file)
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

can& can::can_for_directory(fs::path directory_path, bool fs_trash)
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
	:for_directory{path},
	 fs_trash{fs_trash}
{
	if (fs_trash == false)
	{
		this->path = file{fs::path{user::current().get_XDG_DATA_HOME()} / "Trash"};
		this->create_directory(this->path.as<fs::path>());
	}
	else
	{
		set_top_dir_can_1(std::move(path));
	}

	fs::path files = this->path.as<fs::path>() / "files";
	fs::path info = this->path.as<fs::path>() / "info";
	create_directory(files);
	create_directory(info);
	this->files = file{files};
	this->info = file{info};
}

void can::set_top_dir_can_1(fs::path path)
{
	fs::path trash_path{path / ".Trash"};
	if (fs::exists(trash_path) == false)
	{
		this->set_top_dir_can_2(std::move(path));
		return;
	}
	this->path = file{trash_path};
	if (this->path.has_sticky() == false ||
			this->path.is_symlink())
	{
		this->set_top_dir_can_2(std::move(path));
	}
	std::ostringstream oss;
	oss << user::current().get_uid();
	this->path = file{trash_path / oss.str()};
	try
	{
		this->create_directory(this->path.as<fs::path>());
	}
	catch (const std::runtime_error& e)
	{
		this->set_top_dir_can_2(std::move(path));
	}
}

void can::set_top_dir_can_2(fs::path path)
{
	//TODO report to admin

	boost::system::error_code ec;
	std::ostringstream oss;

	oss << ".Trash-" << user::current().get_uid();

	this->path = file{path / oss.str()};
	this->create_directory(this->path.as<fs::path>());
}

void can::create_directory(const fs::path& path)
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
	file directory{path};
	if (directory.is_directory() == false)
	{
		throw std::runtime_error{std::string("File ") + directory.as<std::string>() + " is not a directory"};
	}
	if (directory.is_writeable_by(user::current()) == false)
	{
		throw std::runtime_error{std::string("File ") + directory.as<std::string>() + " is not writeable"};
	}
}

bool can::put(const fs::path& path)
{
	int i = 0;
	std::ostringstream oss;
	std::string name;
	do
	{
		oss.str("");
		oss << path.filename().string()
			<< " - " << trashinfo::get_current_time()
			<< " - " << i;
		name = oss.str();
		oss << ".trashinfo";
		try
		{
			trashinfo::create(this->info.as<fs::path>() / oss.str(), path);
			i = 0;
		}
		catch(const std::runtime_error& e)
		{
			i++;
		}
	} while (i > 0);

	boost::system::error_code ec;

	fs::rename(fs::absolute(path), this->files.as<fs::path>() / name, ec);

	return !ec;
}

bool can::operator==(const fs::path& directory_path)
{
	return this->for_directory == directory_path;
}

