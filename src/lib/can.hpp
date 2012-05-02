#ifndef TRASH_CAN_HPP
#define TRASH_CAN_HPP

#include <boost/filesystem.hpp>
#include <cppmounts.hpp>
#include <vector>
#include <string>
#include "user.hpp"
#include "file.hpp"

namespace fs = ::boost::filesystem;

namespace trash
{
	class can
	{
		public:
			static can& get_for(fs::path file);

			bool operator==(const fs::path& directory_path);

			void put(const fs::path& path);
		private:
			static bool is_in_home(const fs::path& file);
			static can& can_for_directory(fs::path directory_path, bool fs_trash = true);

			can(fs::path path, bool fs_trash = true);

			void set_top_dir_can_1(fs::path path);
			void set_top_dir_can_2(fs::path path);

			void create_directory(const fs::path& path);

			static std::vector<can> trashcans;

			file path;
			file files;
			file info;
			fs::path for_directory;
			bool fs_trash;
	};
}

#endif /* TRASH_CAN_HPP */

