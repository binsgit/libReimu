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

#include "IPEndPoint.hpp"

Reimu::IPEndPoint::IPEndPoint() {

}

Reimu::IPEndPoint::IPEndPoint(int af) {
	if (af != AF_INET && af != AF_INET6)
		throw Reimu::Exception(EPFNOSUPPORT);

	AddressFamily = af;
}

Reimu::IPEndPoint::IPEndPoint(std::pair<void *, size_t> inaddr, uint16_t port) {
	InitFromRaw(inaddr.first, inaddr.second, port);
}

Reimu::IPEndPoint::IPEndPoint(void *inaddr, size_t inaddr_len, uint16_t port) {
	InitFromRaw(inaddr, inaddr_len, port);
}

void Reimu::IPEndPoint::InitFromRaw(void *inaddr, size_t inaddr_len, uint16_t port) {
	_SockAddr = (union sa_inetall *)malloc(sizeof(union sa_inetall));

	if (inaddr_len == 4) {
		AddressFamily = AF_INET;
		_SockAddr->v4.sin_port = htons(port);
		Port_N = &_SockAddr->v4.sin_port;
		memcpy(&_SockAddr->v4.sin_addr, inaddr, 4);
		SockAddr = (sockaddr *)&_SockAddr->v4;
		Addr = (uint8_t *)&_SockAddr->v4.sin_addr;
		_SockAddr->v4.sin_family = AF_INET;
	} else if (inaddr_len == 16) {
		AddressFamily = AF_INET6;
		_SockAddr->v6.sin6_port = htons(port);
		Port_N = &_SockAddr->v6.sin6_port;
		memcpy(&_SockAddr->v6.sin6_addr, inaddr, 16);
		SockAddr = (sockaddr *)&_SockAddr->v6;
		Addr = (uint8_t *)&_SockAddr->v6.sin6_addr;
		_SockAddr->v6.sin6_family = AF_INET6;
	}

	Port = port;
}

Reimu::IPEndPoint::IPEndPoint(sockaddr_in *sa4) {
	_SockAddr = (union sa_inetall *)malloc(sizeof(union sa_inetall));

	AddressFamily = AF_INET;
	memcpy(&_SockAddr->v4, sa4, sizeof(sockaddr_in));
	Port_N = &_SockAddr->v4.sin_port;
	Port = ntohs(_SockAddr->v4.sin_port);
	SockAddr = (sockaddr *)&_SockAddr->v4;
	Addr = (uint8_t *)&_SockAddr->v4.sin_addr;
	_SockAddr->v4.sin_family = AF_INET;
}

Reimu::IPEndPoint::IPEndPoint(sockaddr_in6 *sa6) {
	_SockAddr = (union sa_inetall *)malloc(sizeof(union sa_inetall));

	AddressFamily = AF_INET6;
	memcpy(&_SockAddr->v6, sa6, sizeof(sockaddr_in6));
	Port_N = &_SockAddr->v6.sin6_port;
	Port = ntohs(_SockAddr->v6.sin6_port);
	SockAddr = (sockaddr *)&_SockAddr->v6;
	Addr = (uint8_t *)&_SockAddr->v6.sin6_addr;
	_SockAddr->v6.sin6_family = AF_INET6;
}

Reimu::IPEndPoint::IPEndPoint(std::string ip_str, uint16_t port) {
	_SockAddr = (union sa_inetall *)malloc(sizeof(union sa_inetall));

	if (strchr(ip_str.c_str(), ':')) {
		AddressFamily = AF_INET6;
		Addr = (uint8_t *)&_SockAddr->v6.sin6_addr;
		Port_N = &_SockAddr->v6.sin6_port;
		SockAddr = (sockaddr *)&_SockAddr->v6;
		_SockAddr->v6.sin6_family = AF_INET6;
	} else {
		AddressFamily = AF_INET;
		Addr = (uint8_t *)&_SockAddr->v4.sin_addr;
		Port_N = &_SockAddr->v4.sin_port;
		SockAddr = (sockaddr *)&_SockAddr->v4;
		_SockAddr->v4.sin_family = AF_INET;
	}

	inet_pton(AddressFamily, ip_str.c_str(), Addr);

	*Port_N = htons(port);
	Port = port;
}

std::string Reimu::IPEndPoint::ToString(Reimu::IPEndPoint::ArgType t) {
	char sbuf[INET6_ADDRSTRLEN] = {0};

	if (t == String_IP) {
		memset(sbuf, 0, INET6_ADDRSTRLEN);
		inet_ntop(AddressFamily, Addr, sbuf, INET6_ADDRSTRLEN);
		std::string ret;
		ret += sbuf;
		return ret;
	} else {
		if (!Port)
			Port = ntohs(*Port_N);
		return std::to_string(Port);
	}
}

std::string Reimu::IPEndPoint::ToString() {
	std::string ret;
	if (AddressFamily == AF_INET6) {
		ret += '[' + ToString(String_IP) + "]";
	} else {
		ret += ToString(String_IP);
	}
	ret += ":" + ToString(String_Port);

	return ret;
}

int Reimu::IPEndPoint::Connect(int ext_socket_type, int ext_socket_protocol) {
	FD_Socket = socket(AddressFamily, SOCK_STREAM|ext_socket_protocol, 0|ext_socket_protocol);

	if (FD_Socket == -1) {
		throw Reimu::Exception(errno);
	}

	if ( 0 != connect(FD_Socket, (AddressFamily == AF_INET6) ?
				     (struct sockaddr *)&_SockAddr->v6 : (struct sockaddr *)&_SockAddr->v4,
			  (AddressFamily == AF_INET6) ? sizeof(struct sockaddr_in6) : sizeof(struct sockaddr_in)) ) {
		throw Reimu::Exception(errno);
	}

	return FD_Socket;
}

void Reimu::IPEndPoint::Close() {
	close(FD_Socket);
}

bool const Reimu::IPEndPoint::operator==(const Reimu::IPEndPoint &o) const {
	if (AddressFamily == AF_INET6) {
		return 0 == memcmp(&_SockAddr->v6, &o._SockAddr->v6, sizeof(sockaddr_in6));
	} else {
		return 0 == memcmp(&_SockAddr->v4, &o._SockAddr->v4, sizeof(sockaddr_in));
	}
}

bool const Reimu::IPEndPoint::operator<(const Reimu::IPEndPoint &o) const {
	if (AddressFamily == AF_INET6) {
		uint64_t thisip[2], thatip[2];

		memcpy(thisip, Addr, 16);
		memcpy(thatip, o.Addr, 16);

		thisip[0] = be64toh(thisip[0]);
		thisip[1] = be64toh(thisip[1]);
		thatip[0] = be64toh(thatip[0]);
		thatip[1] = be64toh(thatip[1]);

		if (thisip[0] < thatip[0])
			return true;
		else if (thisip[0] > thatip[0])
			return false;
		else {
			if (thisip[1] < thatip[1])
				return true;
			else if (thisip[1] > thatip[1])
				return false;
			else {
				return be16toh(*Port_N) < be16toh(*o.Port_N);
			}
		}
	} else {
		uint32_t thisip, thatip;

		memcpy(&thisip, Addr, 4);
		memcpy(&thatip, o.Addr, 4);

		thisip = be32toh(thisip);
		thatip = be32toh(thatip);

		if (thisip < thatip)
			return true;
		else if (thisip > thatip)
			return false;
		else {
			return be16toh(*Port_N) < be16toh(*o.Port_N);
		}
	}
}

Reimu::IPEndPoint &Reimu::IPEndPoint::operator=(Reimu::IPEndPoint other) {
	_SockAddr = other._SockAddr;
	AddressFamily = other.AddressFamily;
	Port = other.Port;
	FD_Socket = other.FD_Socket;
	Port_N = other.Port_N;
	Addr = other.Addr;
	SockAddr = other.SockAddr;
	other._SockAddr = NULL;

	return *this;
}

Reimu::IPEndPoint::~IPEndPoint() {
	if (_SockAddr)
		free(_SockAddr);
}

Reimu::IPEndPoint::IPEndPoint(const Reimu::IPEndPoint &other) {
	_SockAddr = (union sa_inetall *)malloc(sizeof(union sa_inetall));
	memcpy(_SockAddr, other._SockAddr, sizeof(_SockAddr));
	AddressFamily = other.AddressFamily;
	Port = other.Port;
	FD_Socket = other.FD_Socket;

	if (AddressFamily == AF_INET) {
		Port_N = &_SockAddr->v4.sin_port;
		SockAddr = (sockaddr *) &_SockAddr->v4;
		Addr = (uint8_t *) &_SockAddr->v4.sin_addr;
	} else {
		Port_N = &_SockAddr->v6.sin6_port;
		SockAddr = (sockaddr *) &_SockAddr->v6;
		Addr = (uint8_t *) &_SockAddr->v6.sin6_addr;
	}

}

Reimu::IPEndPoint::IPEndPoint(const Reimu::IPEndPoint &&other) {
	_SockAddr = other._SockAddr;
	AddressFamily = other.AddressFamily;
	Port = other.Port;
	FD_Socket = other.FD_Socket;
	Port_N = other.Port_N;
	Addr = other.Addr;
	SockAddr = other.SockAddr;

}

std::string Reimu::IPEndPoint::ToString(in6_addr *addr) {
	char buf[INET6_ADDRSTRLEN];
	inet_ntop(AF_INET6, addr, buf, INET6_ADDRSTRLEN);

	return std::string(buf);
}

std::string Reimu::IPEndPoint::ToString(in_addr *addr) {
	char buf[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, addr, buf, INET_ADDRSTRLEN);

	return std::string(buf);
}

std::string Reimu::IPEndPoint::ToString(in_addr_t addr) {
	char buf[INET_ADDRSTRLEN];
	struct in_addr abuf;
	abuf.s_addr = addr;
	inet_ntop(AF_INET, &abuf, buf, INET_ADDRSTRLEN);

	return std::string(buf);
}


