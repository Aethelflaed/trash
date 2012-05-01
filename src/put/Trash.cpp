#include "Trash.hpp"
#include <string.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <sstream>
#include <unistd.h>

using namespace ::trash;

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

	for (const std::string& file : static_cast<Options*>(this->opts.get())->getInputFiles())
	{
		fs::path path = this->check(file);
		remove_file(path);
	}
	return status;
}

void Trash::check_interactive_once()
{
	if (static_cast<Options*>(this->opts.get())->getInteractive() == Options::Interactive::once)
	{
		std::string msg;
		if (static_cast<Options*>(this->opts.get())->isRecursive())
		{
			msg = "remove all arguments recursively?";
		}
		else if (static_cast<Options*>(this->opts.get())->getInputFiles().size() > 3)
		{
			msg = "remove all arguments?";
		}
		if (msg.size() > 0 && ask(msg) == false)
		{
			exit(0);
		}
	}
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
	else if (static_cast<Options*>(this->opts.get())->isUnlink())
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
		if (static_cast<Options*>(this->opts.get())->isVerbose())
		{
			message("removed ‘"_s + path.string() + "’\n");
		}
	}
	else
	{
		this->report(this->cannot_remove(path, "Permission denied"));
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
	if (static_cast<Options*>(this->opts.get())->isForce() == false ||
		static_cast<Options*>(this->opts.get())->getInteractive() == Options::Interactive::always)
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
			static_cast<Options*>(this->opts.get())->getInteractive() == Options::Interactive::always)
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

fs::path Trash::check(const std::string& file)
{
	fs::path path{file};
	if (fs::exists(path) == false)
	{
		if (static_cast<Options*>(this->opts.get())->isForce() == false)
		{
			this->report(this->cannot_remove(file, "No such file or directory"));
		}
		return fs::path{};
	}
	if (fs::is_directory(path) && static_cast<Options*>(this->opts.get())->isRecursive() == false)
	{
		this->report(this->cannot_remove(file, "Is a directory"));
		return fs::path{};
	}
	if (file == "." ||
		file == ".." ||
		boost::algorithm::ends_with(file, "/.") ||
		boost::algorithm::ends_with(file, "/.."))
	{
		report("cannot remove directory: ‘"_s + file + "’");
		return fs::path{};
	}

	return path;
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

