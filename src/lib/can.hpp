#ifndef TRASH_CAN_HPP
#define TRASH_CAN_HPP

#include <boost/filesystem.hpp>
#include <cppmounts.hpp>
#include <vector>
#include <string>
#include "user.hpp"

namespace fs = ::boost::filesystem;

namespace trash
{
	class can
	{
		public:
			static can& get_for(fs::path file);

			const fs::path& getPath() const noexcept;

			operator const fs::path&();
			operator const std::string&();
			bool operator==(const fs::path& directory_path);

			void put(const fs::path& path);
		private:
			static bool isInHome(const fs::path& file);
			static can& getCanInDirectory(fs::path directory_path, bool fs_trash = true);

			can(fs::path path, bool fs_trash = true);
			void setCanInTopDirectory(const fs::path& path, bool dotTrash = true);
			void createDirectory();
			void createDirectory(const fs::path& path);

			static std::vector<can> trashcans;

			fs::path path;
			fs::path directory;
			bool fs_trash;
	};
}

#endif /* TRASH_CAN_HPP */

