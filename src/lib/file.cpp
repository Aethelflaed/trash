#include "file.hpp"
#include "string.hpp"

#include <stdexcept>
#include <utility>

using namespace ::trash;

file::file(fs::path path)
	:path{std::move(path)},
	 status{fs::status(path)},
	 stat_info{new struct stat()}
{
	//stat should return 0
	if (stat(this->as<const char*>(), this->stat_info.get()))
	{
		throw std::runtime_error{std::string("can't stat file ") + this->path.string()};
	}
}

bool file::is_readable_by(const user& user) const noexcept
{
	if (user.get_uid() == this->get_uid())
	{
		return this->get_perms() & 0400;
	}
	else if (user.get_gid() == this->get_gid())
	{
		return this->get_perms() & 0040;
	}
	else
	{
		return this->get_perms() & 0004;
	}
}

bool file::is_writeable_by(const user& user) const noexcept
{
	if (user.get_uid() == this->get_uid())
	{
		return this->get_perms() & 0200;
	}
	else if (user.get_gid() == this->get_gid())
	{
		return this->get_perms() & 0020;
	}
	else
	{
		return this->get_perms() & 0002;
	}
}

bool file::is_executable_by(const user& user) const noexcept
{
	if (user.get_uid() == this->get_uid())
	{
		return this->get_perms() & 0100;
	}
	else if (user.get_gid() == this->get_gid())
	{
		return this->get_perms() & 0010;
	}
	else
	{
		return this->get_perms() & 0001;
	}
}

std::string file::get_type_as_string() const noexcept
{
	switch(this->get_type())
	{
		case fs::regular_file:
			if (this->is_empty())
				return "regular empty file";
			return "regular file";
		case fs::directory_file:
			if (this->is_empty())
				return "empty directory";
			return "directory";
		case fs::symlink_file:
			return "symbolic link";
		case fs::block_file:
			return "block file";
		case fs::character_file:
			return "character file";
		case fs::fifo_file:
			return "fifo file";
		case fs::socket_file:
			return "socket file";
		case fs::status_error:
			return "status error";
		case fs::type_unknown:
		default:
			return "unknown file type";
	}
}

