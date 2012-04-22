#ifndef TRASH_APPLICATION_HPP
#define TRASH_APPLICATION_HPP

#include "Options.hpp"

class Application
{
public:
	Application(int argc, const char** argv) noexcept;

	virtual void run() = 0;

protected:
	const Options& getOptions() const noexcept;

private:
	int argc;
	const char** argv;

	Options options{};
};

#endif /* TRASH_APPLICATION_HPP */

