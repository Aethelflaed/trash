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

	options.store_cli(argc, argv);
	options.notify();

	return 0;
}

