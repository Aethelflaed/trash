#ifndef TRASH_PUT_OPTIONS_HPP
#define TRASH_PUT_OPTIONS_HPP

#include "header.hpp"
#include <boost/program_options.hpp>
#include <string>
#include <options.hpp>

namespace po = ::boost::program_options;

class Options : public ::trash::options
{
public:
	enum class Interactive;

	Options(::trash::application& app) noexcept;

	bool isForce() const noexcept;
	void setForce(bool force) noexcept;

	bool isVerbose() const noexcept;
	void setVerbose(bool verbose) noexcept;

	bool isRecursive() const noexcept;
	void setRecursive(bool recursive) noexcept;

	bool isPreserveRoot() const noexcept;
	void setPreserveRoot(bool preserve_root) noexcept;
	void setNoPreserveRoot(bool no_preserve_root) noexcept;

	bool isOneFileSystem() const noexcept;
	void setOneFileSystem(bool one_file_system) noexcept;

	bool isUnlink() const noexcept;
	void setUnlink(bool unlink) noexcept;

	Interactive getInteractive() const noexcept;
	void setInteractive(Interactive interactive) noexcept;
	void setInteractiveString(const std::string& interactive) noexcept;
	void setInteractiveOnce(bool value) noexcept;
	void setInteractiveAlways(bool value) noexcept;

	const std::vector<std::string>& getInputFiles() const noexcept;
	void setInputFiles(const std::vector<std::string>& input_files) noexcept;

	const std::string& getProgramName() const noexcept;

	enum class Interactive
	{
		never,
		once,
		always,
	};

private:
	virtual void initialize_options() noexcept override;
	virtual void notify_check() override;

	po::typed_value<bool>* make_bool_switch(void (Options::*callback)(bool));

	bool force;
	bool verbose;
	bool recursive;
	bool preserve_root;
	bool one_file_system;
	bool unlink;
	Interactive interactive;
	std::vector<std::string> input_files;
};

#endif /* TRASH_PUT_OPTIONS_HPP */

