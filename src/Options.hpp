#ifndef TRASH_OPTIONS_HPP
#define TRASH_OPTIONS_HPP

#include <boost/program_options.hpp>
#include <iostream>

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
	Options& setInteractiveString(const std::string& interactive) noexcept;
	Options& setInteractiveOnce(bool value) noexcept;
	Options& setInteractiveAlways(bool value) noexcept;

public:
	const std::string& getTrashCan() const noexcept;
	Options& setTrashCan(const std::string& trash_can) noexcept;

	enum class Interactive
	{
		never,
		once,
		always,
	};
private:
	void abort(const char* msg);
	void abort(const std::string& msg);

	void initialize_options() noexcept;
	po::typed_value<bool>* make_bool_switch(void (Options::*callback)(bool));
	po::typed_value<bool>* make_bool_switch(Options& (Options::*callback)(bool) noexcept (true));

	std::string program_name{"trash"};
	std::string copyright{"Written by Geoffroy Planquart <geoffroy@aethelflaed.com>"};

	bool force{false};
	bool verbose{false};
	bool recursive{false};
	bool preserve_root{false};
	bool one_file_system{false};
	bool unlink{false};
	Interactive interactive{Interactive::never};
	std::string trash_can;

	po::variables_map vm;

	po::options_description generic{"Generic options"};
	po::options_description trash_options{"Trash options"};
	po::options_description config{"Configuration"};
	po::options_description hidden{"Hidden options"};
	po::positional_options_description positional;

	po::options_description cli_options{"."};
	po::options_description config_options{"."};
	po::options_description visible_options{"Allowed options"};
};

#endif /* TRASH_OPTIONS_HPP */

