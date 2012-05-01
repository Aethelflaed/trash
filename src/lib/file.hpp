#ifndef TRASH_FILE_HPP
#define TRASH_FILE_HPP

#include <boost/filesystem.hpp>
#include <string>
#include "user.hpp"

extern "C"
{
#include <unistd.h>
#include <sys/stat.h>
}

namespace fs = ::boost::filesystem;

namespace trash
{
	class file
	{
		public:
			file(fs::path path);

			operator const fs::path&() const noexcept
			{ return this->path; }
			operator const std::string&() const noexcept
			{ return this->path.string(); }
			operator const char*() const noexcept
			{ return fs::absolute(this->path).c_str(); }

			template <class T>
				T as()
				{
					return static_cast<T>(*this);
				}

			uid_t get_uid() const noexcept
			{ return this->stat_info->st_uid; }
			gid_t get_gid() const noexcept
			{ return this->stat_info->st_gid; }
			dev_t get_dev() const noexcept
			{ return this->stat_info->st_dev; }

			fs::perms get_perms() const noexcept
			{ return this->status.permissions(); }
			fs::file_type get_type() const noexcept
			{ return this->status.type(); }

			bool has_suid() const noexcept
			{ return this->get_perms() & fs::set_uid_on_exe; }
			bool has_sgid() const noexcept
			{ return this->get_perms() & fs::set_gid_on_exe; }
			bool has_sticky() const noexcept
			{ return this->get_perms() & fs::sticky_bit; }

			bool is_readable_by(const user& user) const noexcept;
			bool is_writeable_by(const user& user) const noexcept;
			bool is_executable_by(const user& user) const noexcept;

		protected:
			fs::path path;
			fs::file_status status;
			std::shared_ptr<struct stat> stat_info;
	};
}

#endif /* TRASH_FILE_HPP */

