#include "Application.hpp"

Application::Application(int argc, const char** argv) noexcept
	:argc(argc),
	 argv(argv)
{
	options.store_cli(argc, argv);
	options.notify();
}

const Options& Application::getOptions() const noexcept
{
	return options;
}

