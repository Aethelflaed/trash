#include "User.hpp"
#include <utility>
#include <unistd.h>

User::User() noexcept
	:uid{geteuid()},
	 gid{getegid()}
{
}

const std::string& User::getName() const noexcept
{
	return name;
}
void User::setName(std::string name) noexcept
{
	this->name = std::move(name);
}

const std::string& User::getHome() const noexcept
{
	return home;
}
void User::setHome(std::string home) noexcept
{
	this->home = std::move(home);
	this->setXDG_DATA_HOME(this->XDG_DATA_HOME);
}

const std::string& User::getXDG_DATA_HOME() const noexcept
{
	return XDG_DATA_HOME;
}
void User::setXDG_DATA_HOME(std::string XDG_DATA_HOME) noexcept
{
	if (XDG_DATA_HOME.empty() && home.empty() == false)
	{
		this->XDG_DATA_HOME = home + ".local/share";
	}
	else
	{
		this->XDG_DATA_HOME = std::move(XDG_DATA_HOME);
	}
}

uid_t User::getUID() const noexcept
{
	return uid;
}
gid_t User::getGID() const noexcept
{
	return gid;
}

