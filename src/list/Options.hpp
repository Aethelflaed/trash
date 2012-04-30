#ifndef TRASH_OPTIONS_HPP
#define TRASH_OPTIONS_HPP

#include <boost/program_options.hpp>
#include <iostream>
#include <string>
#include "User.hpp"

namespace po = ::boost::program_options;

class Options
{
public:
	enum class Interactive;

	Options() noexcept;

	Options(const Options& options) = delete;
	Options(Options&& options) = delete;
	Options& operator=(const Options& options) = delete;
	Options& operator=(Options&& options) = delete;

	void store_cli(int argc, const char** argv) noexcept;
	void store_config(const std::string& file) noexcept;
	void store_environment() noexcept;
	po::variables_map* notify() noexcept;

	friend std::ostream& operator<<(std::ostream& stream, const Options& options);

	std::string try_msg() const;
	std::string usage() const;

	void help(bool value = true);
	void version(bool value = true);

	bool isForce() const noexcept;
	Options& setForce(bool force) noexcept;

	bool isVerbose() const noexcept;
	Options& setVerbose(bool verbose) noexcept;

	bool isRecursive() const noexcept;
	Options& setRecursive(bool recursive) noexcept;

	bool isPreserveRoot() const noexcept;
	Options& setPreserveRoot(bool preserve_root) noexcept;
	Options& setNoPreserveRoot(bool no_preserve_root) noexcept;

	bool isOneFileSystem() const noexcept;
	Options& setOneFileSystem(bool one_file_system) noexcept;

	bool isUnlink() const noexcept;
	Options& setUnlink(bool unlink) noexcept;

	Interactive getInteractive() const noexcept;
	Options& setInteractive(Interactive interactive) noexcept;
private:
	void setInteractiveString(const std::string& interactive) noexcept;
	void setInteractiveOnce(bool value) noexcept;
	void setInteractiveAlways(bool value) noexcept;

public:
	User& getUser() noexcept;

	const std::vector<std::string>& getInputFiles() const noexcept;
private:
	void setInputFiles(const std::vector<std::string>& input_files) noexcept;

public:
	const std::string& getProgramName() const noexcept;

	enum class Interactive
	{
		never,
		once,
		always,
	};
private:
	void setHome(std::string home) noexcept;
	void setUser(std::string user) noexcept;
	void setXdgDatahome(std::string XDG_DATA_HOME) noexcept;

	void abort(const char* msg);
	void abort(const std::string& msg);

	void initialize_options() noexcept;
	po::typed_value<bool>* make_bool_switch(void (Options::*callback)(bool));
	po::typed_value<bool>* make_bool_switch(Options& (Options::*callback)(bool) noexcept (true));

	std::string parse_env(const std::string& variable);

	std::string program_name;
	std::string copyright;

	bool force;
	bool verbose;
	bool recursive;
	bool preserve_root;
	bool one_file_system;
	bool unlink;
	Interactive interactive;
	std::vector<std::string> input_files;

	User user;

	po::variables_map vm;

	po::options_description generic;
	po::options_description trash_options;
	po::options_description config;
	po::options_description hidden;
	po::positional_options_description positional;

	po::options_description cli_options;
	po::options_description config_options;
	po::options_description visible_options;
	po::options_description env_options;
};

#endif /* TRASH_OPTIONS_HPP */

