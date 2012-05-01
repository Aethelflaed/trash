#ifndef TRASH_APPLICATION_HPP
#define TRASH_APPLICATION_HPP

#include "options.hpp"

#include <memory>
#include <string>
#include <iostream>

namespace trash
{
	class application
	{
		public:
			application() noexcept;
			virtual ~application() noexcept;

			application& set_options(options* opts) noexcept;
			application& parse_options(int argc, const char** argv) noexcept;

			application& set_name(std::string name) noexcept;

			virtual int run();

		protected:
			void abort(const std::string& msg);
			void report(const std::string& msg);
			void message(const std::string& msg, std::ostream& stream = std::cout);

			bool ask(const std::string& msg);
			bool is_affirmative(const std::string& response);

			int status;
			std::string name;
			std::shared_ptr<options> opts;
	};
}

#endif /* TRASH_APPLICATION_HPP */

