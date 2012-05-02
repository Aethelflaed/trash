#include "trashinfo.hpp"
#include "string.hpp"
#include <stdexcept>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <utility>
#include <sstream>

extern "C"
{
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
}

using namespace ::trash;

namespace pt = ::boost::posix_time;

/**
 * create trashinfo file
 * throw if file does exist.
 */
trashinfo trashinfo::create(fs::path path)
{
	return trashinfo{path, true};
}

/**
 * open trashinfo file for read.
 * throw if file does not exist.
 */
trashinfo trashinfo::read(fs::path path)
{
	return trashinfo{path};
}

/**
 * open trashinfo file for read.
 * throw if file does not exist.
 */
trashinfo::trashinfo(fs::path path)
	:path{fs::absolute(std::move(path))}
{
}

/**
 * create trashinfo file
 * throw if file does exist.
 */
trashinfo::trashinfo(fs::path path, bool /* create */)
{
	path = fs::absolute(std::move(path));
	int file = open(path.c_str(), O_CREAT | O_EXCL | O_WRONLY, 0640);
	if (file < 0)
	{
		throw std::runtime_error{"Unable to create file "_s + path.string()};
	}

	std::ostringstream oss;
	oss << "[Trash Info]\n"
		<< "Path=" << path.string() << "\n"
		<< "DeletionDate=" << this->getTime() << "\n"
		;
	const char* content = oss.str().c_str();

	if (write(file, content, strlen(content)) == -1)
	{
		close(file);
		throw std::runtime_error{"Error while writing file "_s + path.string()};
	}

	if (close(file) != 0)
	{
		throw std::runtime_error{"Error while closing file "_s + path.string()};
	}
}

std::string trashinfo::getTime()
{
	std::ostringstream oss;
	const pt::ptime now = pt::second_clock::local_time();
	pt::time_facet*const f = new pt::time_facet("%Y%m%dT%H:%M:%S");
	oss.imbue(std::locale(oss.getloc(), f));
	oss << now;
	return oss.str();
}

