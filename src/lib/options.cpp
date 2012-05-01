#include "options.hpp"
#include <string>
#include <vector>
#include <utility>
#include <fstream>
#include <boost/bind.hpp>
#include <boost/algorithm/string/replace.hpp>

using namespace ::trash;

options::options(application& app) noexcept
	:app(app),
	 generic{"Generic options"},
	 config{"Configuration"},
	 cli_options{"."},
	 visible_options{"Allowed options"}
{
}

void options::initialize() noexcept
{
	generic.add_options()
		("help,h", make_bool_switch(&options::help), "produce help message and exit")
		("version,V", make_bool_switch(&options::version), "print version message and exit")
		;

	initialize_options();

	cli_options.add(config).add(generic).add(hidden);
	visible_options.add(config).add(generic);
}

void options::store_cli(int argc, const char** argv) noexcept
{
	try
	{
		po::store(po::command_line_parser(argc, argv)
				.options(cli_options)
				.positional(positional)
				.run(), vm);
	}
	catch (const po::error& e)
	{
		abort(e.what());
	}
}

po::variables_map* options::notify() noexcept
{
	try
	{
		po::notify(vm);
		this->notify_check();
	}
	catch (const po::error& e)
	{
		abort(e.what());
	}
	return &vm;
}

void options::help(bool value)
{
	if (value)
	{
		app.print_help();
	}
}

void options::version(bool value)
{
	if (value)
	{
		app.print_version();
	}
}

void options::abort(const std::string& msg)
{
	app.abort_try(msg);
}

po::typed_value<bool>* options::make_bool_switch(void (options::*callback)(bool))
{
	return po::bool_switch()->notifier(boost::bind(callback, this, _1));
}

