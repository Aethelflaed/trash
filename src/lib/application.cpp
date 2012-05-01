#include "application.hpp"
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
}

application& application::parse_options(int argc, const char** argv) noexcept
{
	if (this->opts != nullptr)
	{
		this->opts->store_cli(argc, argv);
	}
}

application& application::set_name(std::string name) noexcept
{
	this->name = std::move(name);
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

void application::report(const std::string& msg)
{
	this->status++;
	this->message(msg + "\n", std::cerr);
}

void application::message(const std::string& msg, std::ostream& stream)
{
	stream << this->name << ": " << msg << std::flush;
}

