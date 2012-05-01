#ifndef TRASH_APPLICATION_HPP
#define TRASH_APPLICATION_HPP

#include <memory>
#include <string>
#include <iostream>

namespace trash
{
	class options;

	class application
	{
		public:
			application() noexcept;
			virtual ~application() noexcept;

			application& set_options(options* opts) noexcept;
			int start(int argc, const char** argv);

			void abort(const std::string& msg);
			void abort_try(const std::string& msg);
			void report(const std::string& msg);
			void message(const std::string& msg, std::ostream& stream = std::cout);

			void print_help(bool should_exit = true);
			void print_version(bool should_exit = true);

			virtual std::string get_usage() const noexcept;
			virtual std::string get_try_msg() const noexcept;

			virtual std::string get_name() const noexcept = 0;
			virtual std::string get_version() const noexcept;
			virtual std::string get_copyright() const noexcept = 0;

		protected:
			template <class T>
				T* opts_as()
				{
					return static_cast<T*>(this->opts.get());
				}

			virtual int run();

			bool ask(const std::string& msg);
			bool is_affirmative(const std::string& response);

			int status;
			std::shared_ptr<options> opts;
	};
}

#endif /* TRASH_APPLICATION_HPP */

