#ifndef TRASH_TRASH_HPP
#define TRASH_TRASH_HPP

#include "Options.hpp"
#include <boost/filesystem.hpp>
#include <iostream>

namespace fs = ::boost::filesystem;

class Trash
{
public:
	Trash(int argc, const char** argv) noexcept;

	int run();

private:
	void remove_file(fs::path& path);
	void remove_directory(fs::path& path);

	bool prompt(fs::path& path);
	bool ask(const std::string& msg);
	bool affirmative(const std::string& response);

	fs::path check(const std::string& file);

	void abort(const std::string&, const char* msg);
	void report(const std::string&, const char* msg);
	void report_basic(const std::string& msg);
	void report_basic(const char* msg);
	void message(const std::string& msg, std::ostream& stream = std::cout);

	Options options{};
	int status{0};
};

#endif /* TRASH_TRASH_HPP */

