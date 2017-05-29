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


#include "FileSystem.hpp"

bool Reimu::FileSystem::Exist(std::string path) {
	struct stat OqO;
	return stat(path.c_str(), &OqO) == 0;
}

bool Reimu::FileSystem::MkDir_P(std::string path) {
	return false;
}

inline void Reimu::FileSystem::listdir(const char *base_path, std::vector<std::string> &list, ssize_t plen) {
		DIR *dir;
		struct dirent *entry;

		if (!(dir = opendir(base_path)))
			return;
		if (!(entry = readdir(dir)))
			return;

		char path_buf[1024+plen];

		do {
			if (entry->d_type == DT_DIR) {
				int len = snprintf(path_buf, sizeof(path_buf)-1, "%s/%s", base_path, entry->d_name);
				path_buf[len] = 0;
				if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
					continue;
				listdir(path_buf, list, len);
			}
			else {
				if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
					continue;
				int len = snprintf(path_buf, sizeof(path_buf)-1, "%s/%s", base_path, entry->d_name);
				path_buf[len] = 0;
				list.push_back(path_buf);
			}
		} while (entry = readdir(dir));
		closedir(dir);
}

std::vector<std::string> Reimu::FileSystem::FileList(std::string path) {
	std::vector<std::string> list;

	listdir(path.c_str(), list, 0);

	return list;
}
