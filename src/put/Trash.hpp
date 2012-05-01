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

	virtual int run() override;

	virtual std::string get_usage() const noexcept override;
	virtual std::string get_try_msg() const noexcept override;

	virtual std::string get_name() const noexcept override;
	virtual std::string get_version() const noexcept override;
	virtual std::string get_copyright() const noexcept override;
private:
	void check_interactive_once();

	void remove_file(const fs::path& path);
	void remove_directory(const fs::path& path);

	void delete_file(const fs::path& path);
	void move_file(const fs::path& path);

	std::string getTime();

	bool prompt(const fs::path& path);

	fs::path check(const std::string& file);

	std::string cannot_remove(const std::string& filename, const std::string& msg);
	std::string cannot_remove(const fs::path& path, const std::string& msg);

	std::string trash_can;
};

#endif /* TRASH_PUT_TRASH_HPP */

