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

    IPEndPoint - A new way to hold a network address. Borrowed from C#.
 */

#ifndef LIBREIMU_IPENDPOINT_HPP
#define LIBREIMU_IPENDPOINT_HPP

#include "../libReimu.hpp"
#include "../Exception/Exception.hpp"

namespace Reimu {
    class IPEndPoint {
    public:
	enum ArgType {
	    String_IP = 1, String_Port = 2
	};

	int AddressFamily = AF_INET;

	uint8_t *Addr;
	in_port_t *Port_N;
	in_port_t Port = 0;

	union {
	    sockaddr_in v4;
	    sockaddr_in6 v6;
	} SockAddr;

	int FD_Socket = -1;

	bool const operator== (const Reimu::IPEndPoint &o) const;
	bool const operator< (const Reimu::IPEndPoint &o) const;

	IPEndPoint();
	IPEndPoint(int af);
	IPEndPoint(sockaddr_in *sa4);
	IPEndPoint(sockaddr_in6 *sa6);
	IPEndPoint(void *inaddr, size_t inaddr_len, uint16_t port);
	IPEndPoint(std::string ip_str, uint16_t port);

	int Connect(int ext_socket_type=0, int ext_socket_protocol=0);
	void Close();

	std::string ToString();
	std::string ToString(ArgType t);
    };
}


#endif //LIBREIMU_IPENDPOINT_HPP
