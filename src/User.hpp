#ifndef TRASH_USER_HPP
#define TRASH_USER_HPP

#include <unistd.h>
#include <sys/types.h>
#include <string>

class User
{
public:
	User() noexcept;

	const std::string& getName() const noexcept;
	void setName(std::string name) noexcept;

	const std::string& getHome() const noexcept;
	void setHome(std::string home) noexcept;

	const std::string& getXDG_DATA_HOME() const noexcept;
	void setXDG_DATA_HOME(std::string XDG_DATA_HOME) noexcept;

	uid_t getUID() const noexcept;
	gid_t getGID() const noexcept;
private:
	std::string name;
	std::string home;
	std::string XDG_DATA_HOME;

	uid_t uid{geteuid()};
	gid_t gid{getegid()};
};

#endif /* TRASH_USER_HPP */

