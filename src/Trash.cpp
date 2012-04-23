#include "Trash.hpp"
#include <boost/algorithm/string/predicate.hpp>
#include <boost/filesystem/fstream.hpp>
#include <unistd.h>

Trash::Trash(int argc, const char** argv) noexcept
{
	options.store_cli(argc, argv);
	options.notify();
	if (isatty(fileno(stdin)))
	{
		options.setForce(true);
	}
}

int Trash::run()
{
	if (options.getInteractive() == Options::Interactive::once)
	{
		std::string msg;
		if (options.isRecursive())
		{
			msg = "remove all arguments recursively?";
		}
		else
		{
			msg = "remove all arguments?";
		}
		if (ask(msg) == false)
		{
			exit(0);
		}
	}

	for (const std::string& file : options.getInputFiles())
	{
		fs::path path = this->check(file);
		if (path.empty() == false && this->prompt(path))
		{
			remove_file(path);
		}
	}
	return status;
}

void Trash::remove_file(fs::path& path)
{
	if (fs::is_directory(path))
	{
		remove_directory(path);
		return;
	}
}

void Trash::remove_directory(fs::path& path)
{
}

bool Trash::prompt(fs::path& path)
{
	if (options.isForce() == false ||
		options.getInteractive() == Options::Interactive::always)
	{
		fs::ofstream ostream{path};
		fs::ifstream istream{path};

		if (ostream.fail() ||
			options.getInteractive() == Options::Interactive::always)
		{
			std::string msg{"remove "};
			if (istream.fail() == false)
			{
				msg += "write-protected ";
			}
			if (fs::is_directory(path))
			{
				msg += "directory ";
			}
			if (fs::is_regular_file(path))
			{
				msg += "regular ";
				if (fs::file_size(path) == 0)
					msg += "empty ";
				msg += "file ";
			}
			msg += "‘" + path.native() + "’?";
			return ask(msg);
		}
	}
	return true;
}

bool Trash::ask(const std::string& msg)
{
	message(msg + " ", std::cout);
	std::string response;
	std::cin >> response;
	return affirmative(response);
}

bool Trash::affirmative(const std::string& response)
{
	return response == "yes" || response == "y";
}

fs::path Trash::check(const std::string& file)
{
	fs::path path{file};
	if (fs::exists(path) == false)
	{
		if (options.isForce() == false)
		{
			report(file, "No such file or directory");
		}
		return fs::path{};
	}
	if (fs::is_directory(path) && options.isRecursive() == false)
	{
		report(file, "Is a directory");
		return fs::path{};
	}
	if (file == "." ||
		file == ".." ||
		boost::algorithm::ends_with(file, "/.") ||
		boost::algorithm::ends_with(file, "/.."))
	{
		report_basic(std::string("cannot remove directory: ‘") + file + "’");
		return fs::path{};
	}

	return path;
}

void Trash::abort(const std::string& file, const char* msg)
{
	report(file, msg);
	exit(status);
}

void Trash::report(const std::string& file, const char* msg)
{
	report_basic(std::string("cannot remove ‘") + file
			+ "’: " + msg);
}

void Trash::report_basic(const std::string& msg)
{
	status++;
	message(msg + "\n", std::cerr);
}

void Trash::report_basic(const char* msg)
{
	report_basic(std::string(msg));
}

void Trash::message(const std::string& msg, std::ostream& stream)
{
	stream << options.getProgramName()
		<< ": " << msg << std::flush;
}

