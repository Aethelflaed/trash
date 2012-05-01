#ifndef TRASH_OPTIONS_HPP
#define TRASH_OPTIONS_HPP

#include "application.hpp"
#include <boost/program_options.hpp>
#include <iostream>
#include <string>

namespace po = ::boost::program_options;

namespace trash
{
	class options
	{
		public:
			enum class Interactive;

			options(application& app) noexcept;

			options(const options& options) = delete;
			options(options&& options) = delete;
			options& operator=(const options& options) = delete;
			options& operator=(options&& options) = delete;

			void store_cli(int argc, const char** argv) noexcept;
			po::variables_map* notify() noexcept;

			void help(bool value = true);
			void version(bool value = true);

			friend std::ostream& operator<<(std::ostream& stream, const options& options)
			{
				stream << options.visible_options;
				return stream;
			}
		protected:
			void abort(const std::string& msg);

			void initialize() noexcept;
			virtual void initialize_options() noexcept = 0;

			po::typed_value<bool>* make_bool_switch(void (options::*callback)(bool));

			application& app;

			po::variables_map vm;

			po::options_description generic;
			po::options_description config;
			po::options_description hidden;
			po::positional_options_description positional;

			po::options_description cli_options;
			po::options_description visible_options;
	};
}

#endif /* TRASH_OPTIONS_HPP */

