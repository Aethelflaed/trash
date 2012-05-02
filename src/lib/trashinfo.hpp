#ifndef TRASH_TRASINFO_HPP
#define TRASH_TRASINFO_HPP

#include <boost/filesystem.hpp>
#include "file.hpp"

namespace fs = ::boost::filesystem;

namespace trash
{
	class trashinfo
	{
		public:
			static trashinfo create(fs::path path);
			static trashinfo read(fs::path path);

		private:
			trashinfo(fs::path path);
			trashinfo(fs::path path, bool /* create */);

			std::string getTime();

			file path;
	};
}

#endif /* TRASH_TRASINFO_HPP */

