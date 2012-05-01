#include "application.hpp"
#include "options.hpp"
#include "string.hpp"
#include <utility>

using namespace ::trash;

application::application() noexcept
	:status{0},
	 opts{}
{
}

application::~application() noexcept
{
}

application& application::set_options(options* opts) noexcept
{
	this->opts.reset(opts);
	return *this;
}

application& application::parse_options(int argc, const char** argv) noexcept
{
	if (this->opts != nullptr)
	{
		this->opts->store_cli(argc, argv);
	}
	return *this;
}

int application::run()
{
	return this->status;
}

bool application::ask(const std::string& msg)
{
	this->message(msg + " ");
	std::string response;
	std::cin >> response;
	return this->is_affirmative(response);
}

bool application::is_affirmative(const std::string& response)
{
	return response == "yes" || response == "y";
}

void application::abort(const std::string& msg)
{
	this->report(msg);
	exit(this->status);
}

void application::abort_try(const std::string& msg)
{
	this->report(msg);
	this->message(get_try_msg());
	exit(this->status);
}

void application::report(const std::string& msg)
{
	this->status++;
	this->message(msg + "\n", std::cerr);
}

void application::message(const std::string& msg, std::ostream& stream)
{
	stream << this->get_name() << ": " << msg << std::flush;
}

void application::print_help(bool should_exit)
{
	this->message(get_usage() + "\n");
	std::cout << *(this->opts) << std::endl;
	if (should_exit)
	{
		exit(this->status);
	}
}

void application::print_version(bool should_exit)
{
	std::cout << this->get_name() << " " << this->get_version() << std::endl;
	std::cout << this->get_copyright() << std::endl;
	if (should_exit)
	{
		exit(this->status);
	}
}

std::string application::get_usage() const noexcept
{
	return "usage `"_s + this->get_name() + " [OPTIONS]`";
}
std::string application::get_try_msg() const noexcept
{
	return "Try `"_s + this->get_name() + " --help` for more information.";
}

std::string application::get_version() const noexcept
{
	return "1.0";
}

