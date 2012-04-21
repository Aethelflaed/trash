#include <boost/program_options.hpp>
#include <vector>
#include <string>
#include <iostream>
#include "Options.hpp"
#include <utility>

using namespace ::std;

namespace po = ::boost::program_options;

int main(int argc, const char** argv)
{
	Options options;
	options.store_cli(argc, argv);

	po::variables_map* vm;
	vm = options.notify();

	if (vm->count("help"))
	{
		cout << options << "\n";
	}

	return 0;
}

