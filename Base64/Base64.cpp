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

#include "Base64.hpp"

const char Reimu::Base64::b64_table[] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
					  'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
					  'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
					  'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
					  'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
					  'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
					  'w', 'x', 'y', 'z', '0', '1', '2', '3',
					  '4', '5', '6', '7', '8', '9', '+', '/'};

std::string Reimu::Base64::Encode(unsigned char *src, size_t len) {
	int i = 0;
	int j = 0;
	std::string enc;
	unsigned char buf[4];
	unsigned char tmp[3];

	// parse until end of source
	while (len--) {
		// read up to 3 bytes at a time into `tmp'
		tmp[i++] = *(src++);

		// if 3 bytes read then encode into `buf'
		if (3 == i) {
			buf[0] = (tmp[0] & 0xfc) >> 2;
			buf[1] = ((tmp[0] & 0x03) << 4) + ((tmp[1] & 0xf0) >> 4);
			buf[2] = ((tmp[1] & 0x0f) << 2) + ((tmp[2] & 0xc0) >> 6);
			buf[3] = tmp[2] & 0x3f;

			// allocate 4 new byts for `enc` and
			// then translate each encoded buffer
			// part by index from the base 64 index table
			// into `enc' unsigned char array

			for (i = 0; i < 4; ++i) {
				enc.push_back(b64_table[buf[i]]);
			}

			// reset index
			i = 0;
		}
	}

	// remainder
	if (i > 0) {
		// fill `tmp' with `\0' at most 3 times
		for (j = i; j < 3; ++j) {
			tmp[j] = '\0';
		}

		// perform same codec as above
		buf[0] = (tmp[0] & 0xfc) >> 2;
		buf[1] = ((tmp[0] & 0x03) << 4) + ((tmp[1] & 0xf0) >> 4);
		buf[2] = ((tmp[1] & 0x0f) << 2) + ((tmp[2] & 0xc0) >> 6);
		buf[3] = tmp[2] & 0x3f;

		// perform same write to `enc` with new allocation
		for (j = 0; (j < i + 1); ++j) {
			enc.push_back(b64_table[buf[j]]);
		}

		// while there is still a remainder
		// append `=' to `enc'
		while ((i++ < 3)) {
			enc.push_back('=');
		}
	}

	return enc;
}

std::string Reimu::Base64::Encode(std::string src) {
	return Encode((unsigned char *)src.c_str(), src.size());
}

std::string Reimu::Base64::Encode(std::vector<uint8_t> src) {
	return Encode(&src[0], src.size());
}

std::vector<uint8_t> Reimu::Base64::Decode(unsigned char *src, size_t len) {
	int i = 0;
	int j = 0;
	int l = 0;
	std::vector<uint8_t> dec;
	unsigned char buf[3];
	unsigned char tmp[4];

	// parse until end of source
	while (len--) {
		// break if char is `=' or not base64 char
		if ('=' == src[j]) { break; }
		if (!(isalnum(src[j]) || '+' == src[j] || '/' == src[j])) { break; }

		// read up to 4 bytes at a time into `tmp'
		tmp[i++] = src[j++];

		// if 4 bytes read then decode into `buf'
		if (4 == i) {
			// translate values in `tmp' from table
			for (i = 0; i < 4; ++i) {
				// find translation char in `b64_table'
				for (l = 0; l < 64; ++l) {
					if (tmp[i] == b64_table[l]) {
						tmp[i] = l;
						break;
					}
				}
			}

			// decode
			buf[0] = (tmp[0] << 2) + ((tmp[1] & 0x30) >> 4);
			buf[1] = ((tmp[1] & 0xf) << 4) + ((tmp[2] & 0x3c) >> 2);
			buf[2] = ((tmp[2] & 0x3) << 6) + tmp[3];

			// write decoded buffer to `dec'
			for (i = 0; i < 3; ++i) {
				dec.push_back(buf[i]);
			}

			// reset
			i = 0;
		}
	}

	// remainder
	if (i > 0) {
		// fill `tmp' with `\0' at most 4 times
		for (j = i; j < 4; ++j) {
			tmp[j] = '\0';
		}

		// translate remainder
		for (j = 0; j < 4; ++j) {
			// find translation char in `b64_table'
			for (l = 0; l < 64; ++l) {
				if (tmp[j] == b64_table[l]) {
					tmp[j] = l;
					break;
				}
			}
		}

		// decode remainder
		buf[0] = (tmp[0] << 2) + ((tmp[1] & 0x30) >> 4);
		buf[1] = ((tmp[1] & 0xf) << 4) + ((tmp[2] & 0x3c) >> 2);
		buf[2] = ((tmp[2] & 0x3) << 6) + tmp[3];

		// write remainer decoded buffer to `dec'
		for (j = 0; (j < i - 1); ++j) {
			dec.push_back(buf[j]);
		}
	}

	return dec;
}

std::vector<uint8_t> Reimu::Base64::Decode(std::string src) {
	return Decode((unsigned char *)src.c_str(), src.size());
}
