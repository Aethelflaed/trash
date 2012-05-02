#include "Trash.hpp"
#include <string.hpp>
#include <file.hpp>
#include <user.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/filesystem/fstream.hpp>
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
	bool directory = fs::is_directory(path);
	if (can::get_for(path).put(path))
	{
		if (this->opts_as<Options>()->isVerbose())
		{
			std::ostringstream oss;
			oss << "trashed ";
			if (directory)
				oss << "directory: ";
			oss << "‘" << path.string() << "’\n";
			this->message(oss.str());
		}
	}
	else
	{
		this->report(this->cannot_remove(path, "Permission denied"));
	}
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
	boost::system::error_code ec;
	bool directory = fs::is_directory(path);
	fs::remove(path, ec);
	if (!ec)
	{
		if (this->opts_as<Options>()->isVerbose())
		{
			std::ostringstream oss;
			oss << "removed ";
			if (directory)
				oss << "directory: ";
			oss << "‘" << path.string() << "’\n";
			this->message(oss.str());
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
	if (fs::is_empty(path))
	{
		this->erase_file(path);
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

bool Trash::prompt(fs::path path)
{
	trash::file file{std::move(path)};

	if ((this->opts_as<Options>()->isForce() == false &&
				file.is_writeable_by(user::current()) == false) ||
		this->opts_as<Options>()->getInteractive() == Options::Interactive::always)
	{
		std::ostringstream oss;
		if (file.is_directory() &&
				file.is_empty() == false &&
				this->opts_as<Options>()->isUnlink())
		{
			oss << "descend into ";
			if (file.is_writeable_by(user::current()) == false)
				oss << "write-protected ";
			oss << file.get_type_as_string();
		}
		else
		{
			if (this->opts_as<Options>()->isUnlink())
				oss << "remove ";
			else
				oss << "trash ";
			if (file.is_writeable_by(user::current()) == false)
				oss << "write-protected ";
			oss << file.get_type_as_string();
		}
		oss << " ‘" << file.as<std::string>() << "’?";
		return ask(oss.str());
	}
	return true;
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

