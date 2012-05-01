#include "Options.hpp"
#include <string>
#include <vector>
#include <utility>
#include <fstream>
#include <boost/bind.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <string.hpp>

using namespace ::trash;

Options::Options(application& app) noexcept
	:options{app},
	 force{false},
	 verbose{false},
	 recursive{false},
	 preserve_root{true},
	 one_file_system{false},
	 unlink{false},
	 interactive{Interactive::never}
{
}

void Options::notify_check()
{
	if (this->vm.count("input-file") == 0)
	{
		throw po::error(this->app.get_usage());
	}
}

bool Options::isForce() const noexcept
{
	return force;
}
void Options::setForce(bool force) noexcept
{
	this->force = force;
	if (force)
	{
		setInteractive(Interactive::never);
	}
}

bool Options::isVerbose() const noexcept
{
	return verbose;
}
void Options::setVerbose(bool verbose) noexcept
{
	this->verbose = verbose;
}

bool Options::isRecursive() const noexcept
{
	return recursive;
}
void Options::setRecursive(bool recursive) noexcept
{
	this->recursive = recursive;
}

bool Options::isPreserveRoot() const noexcept
{
	return preserve_root;
}
void Options::setPreserveRoot(bool preserve_root) noexcept
{
	this->preserve_root = preserve_root;
}
void Options::setNoPreserveRoot(bool no_preserve_root) noexcept
{
	this->preserve_root = ! preserve_root;
}

bool Options::isOneFileSystem() const noexcept
{
	return one_file_system;
}
void Options::setOneFileSystem(bool one_file_system) noexcept
{
	this->one_file_system = one_file_system;
}

bool Options::isUnlink() const noexcept
{
	return unlink;
}
void Options::setUnlink(bool unlink) noexcept
{
	this->unlink = unlink;
}

Options::Interactive Options::getInteractive() const noexcept
{
	return interactive;
}
void Options::setInteractive(Interactive interactive) noexcept
{
	this->interactive = interactive;
	if (interactive != Interactive::never)
	{
		this->setForce(false);
	}
}
void Options::setInteractiveString(const std::string& interactive) noexcept
{
	if (interactive == "once")
	{
		setInteractive(Interactive::once);
	}
	else if (interactive == "always")
	{
		setInteractive(Interactive::always);
	}
	else if (interactive == "never")
	{
		setInteractive(Interactive::never);
	}
	else
	{
		abort("invalid parameter for --interactive -- "_s + interactive);
	}
}
void Options::setInteractiveOnce(bool value) noexcept
{
	if (value)
	{
		setInteractive(Interactive::once);
	}
}
void Options::setInteractiveAlways(bool value) noexcept
{
	if (value)
	{
		setInteractive(Interactive::always);
	}
}

const std::vector<std::string>& Options::getInputFiles() const noexcept
{
	return input_files;
}
void Options::setInputFiles(const std::vector<std::string>& input_files) noexcept
{
	this->input_files = input_files;
}

void Options::initialize_options() noexcept
{
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
		("unlink,u", make_bool_switch(&Options::setUnlink), "unlink (delete) files directly (as rm)")
		;

	hidden.add_options()
		("input-file", po::value<std::vector<std::string>>()->notifier(boost::bind(&Options::setInputFiles, this, _1)), "input file")
		;

	positional.add("input-file", -1);
}

po::typed_value<bool>* Options::make_bool_switch(void (Options::*callback)(bool))
{
	return po::bool_switch()->notifier(boost::bind(callback, this, _1));
}

