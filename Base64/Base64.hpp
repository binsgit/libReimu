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

    Base64 - A non-sucking base64 encoding/decoding library.
*/

/*
 * This module contains code from another project.
 *
 * Original project URL: https://github.com/littlstar/b64.c
 *
 * Original license: MIT (https://github.com/littlstar/b64.c/blob/master/LICENSE)
 *
 * Original copyright info:
 *
 * `encode.c' - b64
 *
 * copyright (c) 2014 joseph werle
 *
 */

#ifndef LIBREIMU_BASE64_HPP
#define LIBREIMU_BASE64_HPP

#include "../libReimu.hpp"

namespace Reimu {
    class Base64 {
    public:
	static const char b64_table[];

	std::string Encode(unsigned char *src, size_t len);
	std::string Encode(std::vector<uint8_t> src);
	std::string Encode(std::string src);

	std::vector<uint8_t> Decode(unsigned char *src, size_t len);
	std::vector<uint8_t> Decode(std::string src);

    };
}

#endif //LIBREIMU_BASE64_HPP
