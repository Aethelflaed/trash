#ifndef TRASH_USER_HPP
#define TRASH_USER_HPP

#include <sys/types.h>
#include <string>

namespace trash
{
	class user
	{
		public:
			static const user& current();

			const std::string& get_USER() const noexcept;

			const std::string& get_HOME() const noexcept;

			const std::string& get_XDG_DATA_HOME() const noexcept;

			uid_t get_uid() const noexcept;
			gid_t get_gid() const noexcept;

		private:
			user() noexcept;

			void set_USER(std::string USER) noexcept;
			void set_HOME(std::string HOME) noexcept;
			void set_XDG_DATA_HOME(std::string XDG_DATA_HOME) noexcept;

			static user* current_user;
			static std::string parse_env(const std::string& var);

			std::string USER;
			std::string HOME;
			std::string XDG_DATA_HOME;

			uid_t uid;
			gid_t gid;
	};
}

#endif /* TRASH_USER_HPP */

