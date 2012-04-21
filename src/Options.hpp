#ifndef TRASH_OPTIONS_HPP
#define TRASH_OPTIONS_HPP

#include <boost/program_options.hpp>
#include <iostream>

namespace po = ::boost::program_options;

class Options
{
public:
	Options() noexcept;
	Options(const Options& options) noexcept;
	Options(Options&& options) noexcept;

	Options& operator=(const Options& options) = delete;
	Options& operator=(Options&& options) = delete;

	void store_cli(int argc, const char** argv);
	void store_config(const std::string& file);
	po::variables_map* notify();

	friend std::ostream& operator<<(std::ostream& stream, const Options& options);
private:
	void initialize_options() noexcept;

	po::variables_map vm;

	po::options_description generic;
	po::options_description config;
	po::options_description hidden;
	po::positional_options_description positional;

	po::options_description cli_options;
	po::options_description config_options;
	po::options_description visible_options;
};

#endif /* TRASH_OPTIONS_HPP */

