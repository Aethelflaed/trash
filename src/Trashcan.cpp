#include "Trashcan.hpp"

std::unordered_set<Trashcan> Trashcan::trashcans;

/**
 * Get the trashcan responsible for forFile, for the specified user.
 */
const Trashcan& Trashcan::getTrashcan(fs::path forFile, const User& user)
{
}

/**
 * Initialize a Trashcan in directory pointed by path.
 * The constructor checks the Trash as described by freedesktop.org
 * trash can specifications.
 *
 * The optional parameter fs_trash control whether the trash directory
 * should be hidden or not.
 */
Trashcan::Trashcan(fs::path path, bool fs_trash)
{
}

const fs::path& Trashcan::getPath() const noexcept
{
	return this->path;
}

Trashcan::operator const fs::path&()
{
	return this->path;
}

Trashcan::operator const std::string&()
{
	return this->path.string();
}

