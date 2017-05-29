/*
    This file is part of libReimu.
    Copyright (C) 2016-2017  CloudyReimu <cloudyreimu@gmail.com>

    libReimu is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    libReimu is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with libReimu.  If not, see <http://www.gnu.org/licenses/>.

    FileSystem - File system related functions.

*/

#ifndef LIBREIMU_FILESYSTEM_HPP
#define LIBREIMU_FILESYSTEM_HPP

#include "../libReimu.hpp"

#include <dirent.h>

namespace Reimu {
    class FileSystem {
	static void inline listdir(const char *base_path, std::vector<std::string> &list, ssize_t len);
    public:
	static bool Exist(std::string path);
	static bool MkDir_P(std::string path);
	static std::vector<std::string> FileList(std::string path);
    };


}

#endif //LIBREIMU_FILESYSTEM_HPP
