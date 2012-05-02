#ifndef TRASH_TRASINFO_HPP
#define TRASH_TRASINFO_HPP

#include <boost/filesystem.hpp>

namespace fs = ::boost::filesystem;

namespace trash
{
	class trashinfo
	{
		public:
			static trashinfo create(fs::path file, fs::path path);
			static trashinfo read(fs::path file);
			static std::string get_current_time();

		private:
			trashinfo(fs::path path);
			trashinfo(fs::path file, fs::path path);

			std::string url_encode(std::string string);

			fs::path file;
			std::string deletion_date;
			std::string path;
	};
}

#endif /* TRASH_TRASINFO_HPP */

