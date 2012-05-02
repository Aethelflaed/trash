#include "trashinfo.hpp"
#include "string.hpp"
#include <stdexcept>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/program_options.hpp>
#include <utility>
#include <sstream>
#include <fstream>

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
namespace po = ::boost::program_options;

#include <boost/function_output_iterator.hpp>
#include <boost/bind.hpp>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <iterator>
#include <iomanip>

namespace
{
	std::string encimpl(std::string::value_type v)
	{
		if (isalnum(v))
			return std::string()+v;

		std::ostringstream enc;
		enc << '%' << std::setw(2) << std::setfill('0') << std::hex << std::uppercase << int(static_cast<unsigned char>(v));
		return enc.str();
	}
}

std::string trashinfo::url_encode(std::string string)
{
	std::string str;
	std::transform(string.begin(), string.end(),
			boost::make_function_output_iterator(boost::bind(static_cast<std::string& (std::string::*)(const std::string&)>(&std::string::append), &str, _1)),
			encimpl);
	return str;
}

/**
 * create trashinfo file
 * throw if file does exist.
 */
trashinfo trashinfo::create(fs::path file, fs::path path)
{
	return trashinfo{std::move(file), std::move(path)};
}

/**
 * open trashinfo file for read.
 * throw if file does not exist.
 */
trashinfo trashinfo::read(fs::path file)
{
	return trashinfo{std::move(file)};
}

/**
 * open trashinfo file for read.
 * throw if file does not exist.
 */
trashinfo::trashinfo(fs::path file)
	:file{fs::absolute(std::move(file))}
{
	po::options_description desc;
	desc.add_options()
		("Trash Info.Path", po::value<std::vector<std::string>>(), "")
		("Trash Info.DeletionDate", po::value<std::vector<std::string>>(), "")
		;
	po::variables_map vm;
	std::ifstream stream(this->file.string());
	po::store(po::parse_config_file(stream, desc, true), vm);
	po::notify(vm);

	if (vm.count("Trash Info.Path"))
		this->path = vm["Trash Info.Path"].as<std::vector<std::string>>()[0];
	else
		throw std::runtime_error{"Broken trashinfo file"_s + this->file.string()};

	if (vm.count("Trash Info.DeletionDate"))
		this->path = vm["Trash Info.DeletionDate"].as<std::vector<std::string>>()[0];
	else
		throw std::runtime_error{"Broken trashinfo file"_s + this->file.string()};
}

/**
 * create trashinfo file
 * throw if file does exist.
 */
trashinfo::trashinfo(fs::path file, fs::path path)
	:file{fs::absolute(std::move(file))}
{
	int fildes = open(this->file.c_str(), O_CREAT | O_EXCL | O_WRONLY, 0640);
	if (fildes < 0)
	{
		throw std::runtime_error{"Unable to create file "_s + this->file.string()};
	}

	this->path = this->url_encode(path.string());
	this->deletion_date = this->get_current_time();

	std::ostringstream oss;
	oss << "[Trash Info]\n"
		<< "Path=" << this->path << "\n"
		<< "DeletionDate=" << this->deletion_date << "\n"
		;
	const char* content = oss.str().c_str();

	if (write(fildes, content, strlen(content)) == -1)
	{
		close(fildes);
		throw std::runtime_error{"Error while writing file "_s + this->file.string()};
	}

	if (close(fildes) != 0)
	{
		throw std::runtime_error{"Error while closing file "_s + this->file.string()};
	}
}

std::string trashinfo::get_current_time()
{
	std::ostringstream oss;
	const pt::ptime now = pt::second_clock::local_time();
	pt::time_facet*const f = new pt::time_facet("%Y%m%dT%H:%M:%S");
	oss.imbue(std::locale(oss.getloc(), f));
	oss << now;
	return oss.str();
}

