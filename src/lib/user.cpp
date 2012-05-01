#include "user.hpp"
#include <utility>
#include <unistd.h>
#include <boost/program_options.hpp>
#include <iostream>

using namespace ::trash;
namespace po = ::boost::program_options;

user* user::current_user = nullptr;

const user& user::current()
{
	if (current_user == nullptr)
	{
		po::options_description opts{"env"};
		opts.add_options()
			("HOME", po::value<std::string>(), "")
			("USER", po::value<std::string>(), "")
			("XDG_DATA_HOME", po::value<std::string>(), "")
			;
		po::variables_map vm;

		try
		{
			po::store(po::parse_environment(opts, &user::parse_env), vm);
			po::notify(vm);

			current_user = new user{};
			if (vm.count("HOME"))
			{
				current_user->set_HOME(vm["HOME"].as<std::string>());
			}
			if (vm.count("USER"))
			{
				current_user->set_USER(vm["USER"].as<std::string>());
			}
			if (vm.count("XDG_DATA_HOME"))
			{
				current_user->set_XDG_DATA_HOME(vm["XDG_DATA_HOME"].as<std::string>());
			}
		}
		catch (const po::error& e)
		{
			std::cerr << e.what() << std::endl;
			exit(1);
		}
	}
	return *current_user;
}

std::string user::parse_env(const std::string& var)
{
	if (var == "HOME" || var == "USER" || var == "XDG_DATA_HOME")
	{
		return var;
	}
	return "";
}

user::user() noexcept
	:uid{geteuid()},
	 gid{getegid()}
{
}

const std::string& user::get_USER() const noexcept
{
	return USER;
}
void user::set_USER(std::string USER) noexcept
{
	this->USER = std::move(USER);
}

const std::string& user::get_HOME() const noexcept
{
	return HOME;
}
void user::set_HOME(std::string HOME) noexcept
{
	this->HOME = std::move(HOME);
	this->set_XDG_DATA_HOME(this->XDG_DATA_HOME);
}

const std::string& user::get_XDG_DATA_HOME() const noexcept
{
	return XDG_DATA_HOME;
}
void user::set_XDG_DATA_HOME(std::string XDG_DATA_HOME) noexcept
{
	if (XDG_DATA_HOME.empty() && HOME.empty() == false)
	{
		this->XDG_DATA_HOME = HOME + ".local/share";
	}
	else
	{
		this->XDG_DATA_HOME = std::move(XDG_DATA_HOME);
	}
}

uid_t user::get_uid() const noexcept
{
	return uid;
}
gid_t user::get_gid() const noexcept
{
	return gid;
}

