#include "file.hpp"
#include "string.hpp"

#include <stdexcept>
#include <utility>

using namespace ::trash;

file::file(fs::path path)
	:path{std::move(path)},
	 stat_info{new struct stat()}
{
	//stat should return 0
	if (stat(this->as<const char*>(), this->stat_info.get()))
	{
		throw std::runtime_error{"can't stat file "_s + this->path.string()};
	}
}

file::operator const fs::path&() const noexcept
{
	return this->path;
}
file::operator const std::string&() const noexcept
{
	return this->path.string();
}
file::operator const char*() const noexcept
{
	return fs::absolute(this->path).c_str();
}

uid_t file::get_uid() const noexcept
{
	return this->stat_info->st_uid;
}

gid_t file::get_gid() const noexcept
{
	return this->stat_info->st_gid;
}

dev_t file::get_dev() const noexcept
{
	return this->stat_info->st_dev;
}

