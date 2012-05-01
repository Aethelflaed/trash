#ifndef TRASH_PUT_TRASH_HPP
#define TRASH_PUT_TRASH_HPP

#include <application.hpp>
#include "Options.hpp"
#include <boost/filesystem.hpp>
#include <iostream>
#include <can.hpp>

namespace fs = ::boost::filesystem;

class Trash : public ::trash::application
{
public:
	Trash() noexcept;
	virtual ~Trash() noexcept;

	virtual std::string get_usage() const noexcept override;

	virtual std::string get_name() const noexcept override;
	virtual std::string get_version() const noexcept override;
	virtual std::string get_copyright() const noexcept override;

private:
	virtual int run() override;

	bool check(const fs::path& path);

	void trash(const fs::path& path);

	void erase(const fs::path& path);
	void erase_file(const fs::path& path);
	void erase_directory(const fs::path& path);

	void check_interactive_once();

	bool prompt(fs::path path);

	std::string getTime();

	std::string cannot_remove(const std::string& filename, const std::string& msg);
	std::string cannot_remove(const fs::path& path, const std::string& msg);
};

#endif /* TRASH_PUT_TRASH_HPP */

