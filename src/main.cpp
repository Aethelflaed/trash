#include <boost/program_options.hpp>
#include <vector>
#include <string>
#include <iostream>
#include "Options.hpp"
#include <utility>

namespace po = ::boost::program_options;

int main(int argc, const char** argv)
{
	Options options;

	po::variables_map* vm;

	try
	{
		options.store_cli(argc, argv);
		vm = options.notify();
	}
	catch (const boost::program_options::error& e)
	{
		std::cerr << e.what() << std::endl;
		exit(1);
	}

	return 0;
}

