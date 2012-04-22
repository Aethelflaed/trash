#include "Options.hpp"
#include <string>
#include <vector>
#include <utility>
#include <string>
#include <fstream>
#include <boost/bind.hpp>

Options::Options() noexcept
{
	initialize_options();
}

void Options::store_cli(int argc, const char** argv) noexcept
{
	try
	{
		po::store(po::command_line_parser(argc, argv)
				.options(cli_options)
				.positional(positional)
				.run(), vm);
	}
	catch (const boost::program_options::error& e)
	{
		abort(e.what());
	}
}

void Options::store_config(const std::string& filename) noexcept
{
	try
	{
		std::ifstream file(filename);
		po::store(po::parse_config_file(file, config_options), vm);
	}
	catch (const boost::program_options::error& e)
	{
		abort(e.what());
	}
}

po::variables_map* Options::notify() noexcept
{
	try
	{
		po::notify(vm);
		if (vm.count("input-file") == 0)
		{
			throw po::error(usage());
		}
	}
	catch (const boost::program_options::error& e)
	{
		abort(e.what());
	}
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

std::string Options::usage() const
{
	return std::string("usage `") + program_name + " [OPTIONS] file ...`";
}

void Options::help(bool value)
{
	if (value)
	{
		std::cout << program_name << ": " << usage() << std::endl;
		std::cout << visible_options << std::endl;
		exit(0);
	}
}

void Options::version(bool value)
{
	if (value)
	{
		std::cout << program_name << " 1.0" << std::endl;
		std::cout << copyright << std::endl;
		exit(0);
	}
}

bool Options::isForce() const noexcept
{
	return force;
}
Options& Options::setForce(bool force) noexcept
{
	this->force = force;
	return *this;
}

bool Options::isVerbose() const noexcept
{
	return verbose;
}
Options& Options::setVerbose(bool verbose) noexcept
{
	this->verbose = verbose;
	return *this;
}

bool Options::isRecursive() const noexcept
{
	return recursive;
}
Options& Options::setRecursive(bool recursive) noexcept
{
	this->recursive = recursive;
	return *this;
}

bool Options::isPreserveRoot() const noexcept
{
	return preserve_root;
}
Options& Options::setPreserveRoot(bool preserve_root) noexcept
{
	this->preserve_root = preserve_root;
	return *this;
}
Options& Options::setNoPreserveRoot(bool no_preserve_root) noexcept
{
	this->preserve_root = ! preserve_root;
	return *this;
}

bool Options::isOneFileSystem() const noexcept
{
	return one_file_system;
}
Options& Options::setOneFileSystem(bool one_file_system) noexcept
{
	this->one_file_system = one_file_system;
	return *this;
}

bool Options::isUnlink() const noexcept
{
	return unlink;
}
Options& Options::setUnlink(bool unlink) noexcept
{
	this->unlink = unlink;
	return *this;
}

Options::Interactive Options::getInteractive() const noexcept
{
	return interactive;
}
Options& Options::setInteractive(Interactive interactive) noexcept
{
	this->interactive = interactive;
	return *this;
}
Options& Options::setInteractiveString(const std::string& interactive) noexcept
{
	if (interactive == "once")
	{
		this->interactive = Interactive::once;
	}
	else if (interactive == "always")
	{
		this->interactive = Interactive::always;
	}
	else if (interactive == "never")
	{
		this->interactive = Interactive::never;
	}
	else
	{
		abort(std::string("invalid parameter for --interactive -- ") + interactive);
	}
	return *this;
}
Options& Options::setInteractiveOnce(bool value) noexcept
{
	if (value)
	{
		interactive = Interactive::once;
	}
	return *this;
}
Options& Options::setInteractiveAlways(bool value) noexcept
{
	if (value)
	{
		interactive = Interactive::always;
	}
	return *this;
}

const std::string& Options::getTrashCan() const noexcept
{
	return trash_can;
}
Options& Options::setTrashCan(const std::string& trash_can) noexcept
{
	this->trash_can = trash_can;
	return *this;
}

void Options::abort(const char* msg)
{
	abort(std::string(msg));
}
void Options::abort(const std::string& msg)
{
	std::cerr << program_name << ": " << msg << std::endl;
	std::cerr << try_msg() << std::endl;
	exit(1);
}

void Options::initialize_options() noexcept
{
	generic.add_options()
		("help,h", make_bool_switch(&Options::help), "produce help message and exit")
		("version,V", make_bool_switch(&Options::version), "print version message and exit")
		;

	config.add_options()
		("recursive,r", make_bool_switch(&Options::setRecursive), "remove file hierarchies")
		("folders,R", make_bool_switch(&Options::setRecursive), "remove file hierarchies")
		("verbose,v", make_bool_switch(&Options::setVerbose), "report any action")
		("force,f", make_bool_switch(&Options::setForce), "ignore nonexistent files, never prompt")
		("interactive", po::value<std::string>()->notifier(boost::bind(&Options::setInteractiveString, this, _1)), "prompt according to arg: never, once (-I), or always (-i).  Without arg, prompt always")
		("interactive-once,I", make_bool_switch(&Options::setInteractiveOnce), "prompt once before removing more than three files, or when removing recursively.  Less intrusive than -i, while still giving protection against most mistakes")
		("interactive-always,i", make_bool_switch(&Options::setInteractiveAlways), "prompt before every removal")
		("preserve-root", make_bool_switch(&Options::setPreserveRoot), "fail to operate recursively on '/'")
		("no-preserve-root", make_bool_switch(&Options::setNoPreserveRoot), "do not treat '/' specially (the default)")
		("one-file-system", make_bool_switch(&Options::setOneFileSystem), "when removing a hierarchy recursively, skip any directory that is on a file system different from that of the corresponding command line argument")
		;

	trash_options.add_options()
		("trash,t", po::value<std::string>()->notifier(boost::bind(&Options::setTrashCan, this, _1)), "specify trash can location")
		("unlink,u", make_bool_switch(&Options::setUnlink), "unlink (delete) files directly (as rm)")
		;

	hidden.add_options()
		("input-file", po::value<std::vector<std::string>>(), "input file")
		;

	positional.add("input-file", -1);

	cli_options.add(config).add(trash_options).add(generic).add(hidden);
	config_options.add(config).add(trash_options).add(hidden);
	visible_options.add(config).add(trash_options).add(generic);
}

po::typed_value<bool>* Options::make_bool_switch(void (Options::*callback)(bool))
{
	return po::bool_switch()->notifier(boost::bind(callback, this, _1));
}

po::typed_value<bool>* Options::make_bool_switch(Options& (Options::*callback)(bool) noexcept (true))
{
	return po::bool_switch()->notifier(boost::bind(callback, this, _1));
}

