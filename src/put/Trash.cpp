#include "Trash.hpp"
#include <string.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <sstream>
#include <unistd.h>

using namespace ::trash;

namespace pt = ::boost::posix_time;

Trash::Trash() noexcept
	:application()
{
	this->set_options(new Options(*this));
}

Trash::~Trash() noexcept
{
}

int Trash::run()
{
	this->check_interactive_once();

	for (const std::string& file : this->opts_as<Options>()->getInputFiles())
	{
		fs::path path{file};
		if (this->check(path))
		{
			if (this->opts_as<Options>()->isUnlink())
			{
				this->erase(path);
			}
			else if (this->prompt(path))
			{
				this->trash(path);
			}
		}
	}
	return status;
}

bool Trash::check(const fs::path& path)
{
	if (fs::exists(path) == false)
	{
		if (this->opts_as<Options>()->isForce() == false)
		{
			this->report(this->cannot_remove(path, "No such file or directory"));
		}
		return false;
	}
	if (fs::is_directory(path) && this->opts_as<Options>()->isRecursive() == false)
	{
		this->report(this->cannot_remove(path, "Is a directory"));
		return false;
	}

	std::string file = path.string();

	if (file == "." ||
		file == ".." ||
		boost::algorithm::ends_with(file, "/.") ||
		boost::algorithm::ends_with(file, "/.."))
	{
		report("cannot remove directory: ‘"_s + file + "’");
		return false;
	}

	return true;
}

void Trash::trash(const fs::path& path)
{
	can::get_for(path).put(path);
}

void Trash::erase(const fs::path& path)
{
	if (this->prompt(path))
	{
		if (fs::is_directory(path))
		{
			this->erase_directory(path);
		}
		else
		{
			this->erase_file(path);
		}
	}
}
void Trash::erase_file(const fs::path& path)
{
	if (fs::remove(path))
	{
		if (this->opts_as<Options>()->isVerbose())
		{
			this->message("removed ‘"_s + path.string() + "’\n");
		}
	}
	else
	{
		this->report(this->cannot_remove(path, "Permission denied"));
	}
}
void Trash::erase_directory(const fs::path& path)
{
	fs::directory_iterator end;
	for (fs::directory_iterator it(path); it != end; it++)
	{
		this->erase(it->path());
	}
}

void Trash::check_interactive_once()
{
	if (this->opts_as<Options>()->getInteractive() == Options::Interactive::once)
	{
		std::string msg;
		if (this->opts_as<Options>()->isRecursive())
		{
			msg = "remove all arguments recursively?";
		}
		else if (this->opts_as<Options>()->getInputFiles().size() > 3)
		{
			msg = "remove all arguments?";
		}
		if (msg.size() > 0 && ask(msg) == false)
		{
			exit(0);
		}
	}
}

bool Trash::prompt(const fs::path& path)
{
	if (this->opts_as<Options>()->isForce() == false ||
		this->opts_as<Options>()->getInteractive() == Options::Interactive::always)
	{
		std::string file_type;
		if (fs::is_directory(path))
		{
			if (fs::is_empty(path))
			{
				file_type = "empty ";
			}
			file_type += "directory ";
		}
		else if (fs::is_regular_file(path))
		{
			if (fs::is_empty(path))
			{
				file_type = "regular empty file ";
			}
			else
			{
				file_type = "regular file ";
			}
		}
		else if (fs::is_symlink(path))
		{
			file_type = "symbolic link ";
		}
		else
		{
			file_type = "file ";
		}
		fs::perms permissions = fs::status(path).permissions();

		/*fs::ofstream ostream{path, std::ios_base::app | std::ios_base::out};
		fs::ifstream istream{path};

		if (ostream.fail() ||
			this->opts_as<Options>()->getInteractive() == Options::Interactive::always)
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
		*/
	}
	return true;
}

std::string Trash::getTime()
{
	std::ostringstream oss;
	const pt::ptime now = pt::second_clock::local_time();
	pt::time_facet*const f = new pt::time_facet("%Y-%m-%d %H:%M:%S");
	oss.imbue(std::locale(oss.getloc(), f));
	oss << now;
	return oss.str();
}

std::string Trash::cannot_remove(const std::string& filename, const std::string& msg)
{
	return "cannot remove ‘"_s + filename + "’: " + msg;
}
std::string Trash::cannot_remove(const fs::path& path, const std::string& msg)
{
	return this->cannot_remove(path.string(), msg);
}


std::string Trash::get_usage() const noexcept
{
	return "usage `"_s + this->get_name() + " [OPTIONS] file ...`";
}
std::string Trash::get_name() const noexcept
{
	return "trash-put";
}
std::string Trash::get_version() const noexcept
{
	return "1.0";
}
std::string Trash::get_copyright() const noexcept
{
	return "Written by Geoffroy Planquart <geoffroy@aethelflaed.com>";
}

