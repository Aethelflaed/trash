#include "Trash.hpp"
#include <boost/algorithm/string/predicate.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <sstream>
#include <unistd.h>

using namespace ::trash;

Trash::Trash(int argc, const char** argv) noexcept
	:status{0}
{
	options.store_cli(argc, argv);
	options.notify();

	if (isatty(fileno(stdin)) == false)
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
		else if (options.getInputFiles().size() > 3)
		{
			msg = "remove all arguments?";
		}
		if (msg.size() > 0 && ask(msg) == false)
		{
			exit(0);
		}
	}

	for (const std::string& file : options.getInputFiles())
	{
		fs::path path = this->check(file);
		remove_file(path);
	}
	return status;
}

void Trash::remove_file(const fs::path& path)
{
	if (path.empty() || this->prompt(path) == false)
	{
		return;
	}
	if (fs::is_directory(path))
	{
		remove_directory(path);
	}
	else if (options.isUnlink())
	{
		this->delete_file(path);
	}
	else
	{
		this->move_file(path);
	}
}

void Trash::remove_directory(const fs::path& path)
{
	fs::directory_iterator end;
	for (fs::directory_iterator it(path); it != end; it++)
	{
		this->remove_file(it->path());
	}
}

void Trash::delete_file(const fs::path& path)
{
	if (fs::remove(path))
	{
		if (options.isVerbose())
		{
			message(std::string("removed ‘") + path.string() + "’\n");
		}
	}
	else
	{
		report(path, "Permission denied");
	}
}

void Trash::move_file(const fs::path& path)
{
	can::get_for(path).put(path);
}

namespace pt = ::boost::posix_time;

std::string Trash::getTime()
{
	std::ostringstream oss;
	const pt::ptime now = pt::second_clock::local_time();
	pt::time_facet*const f = new pt::time_facet("%Y-%m-%d %H:%M:%S");
	oss.imbue(std::locale(oss.getloc(), f));
	oss << now;
	return oss.str();
}

bool Trash::prompt(const fs::path& path)
{
	if (options.isForce() == false ||
		options.getInteractive() == Options::Interactive::always)
	{
		//fs::perms permissions = fs::status(path).permissions();
		std::string file_type;
		if (fs::is_directory(path))
		{
			file_type = "directory ";
		}
		if (fs::is_regular_file(path))
		{
			if (fs::file_size(path) == 0)
			{
				file_type = "regular empty file ";
			}
			else
			{
				file_type = "regular file ";
			}
		}

		fs::ofstream ostream{path, std::ios_base::app | std::ios_base::out};
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
void Trash::abort(const fs::path& file, const char* msg)
{
	abort(file.string(), msg);
}
void Trash::abort(const std::string& msg)
{
	report_basic(msg);
	exit(status);
}

void Trash::report(const std::string& file, const char* msg)
{
	report_basic(std::string("cannot remove ‘") + file
			+ "’: " + msg);
}
void Trash::report(const fs::path& file, const char* msg)
{
	report(file.string(), msg);
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

