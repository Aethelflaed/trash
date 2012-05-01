#include "Trash.hpp"

int main(int argc, const char** argv)
{
	return Trash{}.parse_options(argc, argv).run();
}

