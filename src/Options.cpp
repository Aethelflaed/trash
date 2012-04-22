#include "Options.hpp"
#include <string>
#include <vector>
#include <utility>
#include <string>
#include <fstream>
#include <boost/bind.hpp>

Options::Options() noexcept
	:generic("Generic options"),
	 config("Configuration"),
	 hidden("Hidden options"),
	 positional(),
	 cli_options("."),		// May cause pointer being freed was not allocated
	 config_options("."),	// if there is no name
	 visible_options("Allowed options")
{
	initialize_options();
}

Options::Options(const Options& options) noexcept
	:vm(options.vm),
	 generic(options.generic),
	 config(options.config),
	 hidden(options.hidden),
	 positional(options.positional),
	 cli_options(options.cli_options),
	 config_options(options.config_options),
	 visible_options(options.visible_options)
{
}

Options::Options(Options&& options) noexcept
	:vm(std::move(options.vm)),
	 generic(std::move(options.generic)),
	 config(std::move(options.config)),
	 hidden(std::move(options.hidden)),
	 positional(std::move(options.positional)),
	 cli_options(std::move(options.cli_options)),
	 config_options(std::move(options.config_options)),
	 visible_options(std::move(options.visible_options))
{
}

void Options::store_cli(int argc, const char** argv)
{
	po::store(po::command_line_parser(argc, argv)
			.options(cli_options)
			.positional(positional)
			.run(), vm);
}

void Options::store_config(const std::string& filename)
{
	std::ifstream file(filename);
	po::store(po::parse_config_file(file, config_options), vm);
}

po::variables_map* Options::notify()
{
	po::notify(vm);
	return &vm;
}

std::ostream& operator<<(std::ostream& stream, const Options& options)
{
	stream << options.try_msg();
	return stream;
}

std::string Options::try_msg() const
{
	return std::string("Try `") + program_name + " --help` for more information.";
}

void Options::help(bool value)
{
	if (value)
	{
		std::cout << visible_options << std::endl;
		exit(0);
	}
}

void Options::version(bool value)
{
	if (value)
	{
		std::cout << "Version 1.0" << std::endl;
		exit(0);
	}
}

void Options::initialize_options() noexcept
{
	generic.add_options()
		("help,h", make_bool_switch(&Options::help), "produce help message and exit")
		("version,V", make_bool_switch(&Options::version), "print version message and exit")
		;

	config.add_options()
		("trash,t", po::value<std::string>(), "specify trash can location")
		;

	hidden.add_options()
		("input-file", po::value<std::vector<std::string>>(), "input file")
		;

	positional.add("input-file", -1);

	cli_options.add(generic).add(config).add(hidden);
	config_options.add(config).add(hidden);
	visible_options.add(generic).add(config);
}

po::typed_value<bool>* Options::make_bool_switch(void (Options::*callback)(bool))
{
	return po::bool_switch()->notifier(boost::bind(callback, this, _1));
}

