#ifndef TRASH_FILE_HPP
#define TRASH_FILE_HPP

#include <boost/filesystem.hpp>
#include <string>

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
			file(const fs::path& path);

			operator const fs::path&() const noexcept;
			operator const std::string&() const noexcept;
			operator const char*() const noexcept;

			template <class T>
				T as()
				{
					return static_cast<T>(*this);
				}

			uid_t get_uid() const noexcept;
			gid_t get_gid() const noexcept;
			dev_t get_dev() const noexcept;
		private:
			const fs::path& path;
			std::shared_ptr<struct stat> stat_info;
	};
}

#endif /* TRASH_FILE_HPP */

