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
	void remove_file(const fs::path& path);
	void remove_directory(const fs::path& path);

	void delete_file(const fs::path& path);
	void move_file(const fs::path& path);
	void write_properties(const fs::path& path, const std::string& property_file);

	std::string getTime();

	bool prompt(const fs::path& path);
	bool ask(const std::string& msg);
	bool affirmative(const std::string& response);

	fs::path check(const std::string& file);

	void abort(const std::string& file, const char* msg);
	void abort(const fs::path& file, const char* msg);
	void abort(const std::string& msg);
	void report(const std::string& file, const char* msg);
	void report(const fs::path& file, const char* msg);
	void report_basic(const std::string& msg);
	void report_basic(const char* msg);
	void message(const std::string& msg, std::ostream& stream = std::cout);

	void parse_config(int argc, const char** argv);

	Options options{};
	std::string trash_can{};
	int status{0};
};

#endif /* TRASH_TRASH_HPP */

