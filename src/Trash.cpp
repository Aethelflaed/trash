#include "Trash.hpp"

Trash::Trash(int argc, const char** argv) noexcept
{
	options.store_cli(argc, argv);
	options.notify();
}

int Trash::run()
{
	for (const std::string& file : options.getInputFiles())
	{
		fs::path path{file};

		if (fs::exists(path))
		{
			std::cout << "Path " << path << "exists !" << std::endl;
		}
		else
		{
			std::cout << path << " doesn't exist !" << std::endl;
		}

		paths.push_back(path);
	}
	return 0;
}

