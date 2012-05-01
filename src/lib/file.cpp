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
		throw std::runtime_error{"can't stat file "_s + this->path.string()};
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

