//
// Created by root on 17-3-24.
//

#include "IPEndPoint.hpp"

Reimu::IPEndPoint::IPEndPoint(int af) {
	if (af != AF_INET && af != AF_INET6)
		throw Reimu::Exception(EPFNOSUPPORT);

	AddressFamily = af;
}

Reimu::IPEndPoint::IPEndPoint(std::string ip_str, uint16_t port) {
	if (strchr(ip_str.c_str(), ':')) {
		AddressFamily = AF_INET6;
		Addr = (uint8_t *)&SockAddr.v6.sin6_addr;
		Port_N = &SockAddr.v6.sin6_port;
	} else {
		AddressFamily = AF_INET;
		Addr = (uint8_t *)&SockAddr.v4.sin_addr;
		Port_N = &SockAddr.v4.sin_port;
	}

	inet_pton(AddressFamily, ip_str.c_str(), Addr);
	*Port_N = htons(port);
	Port = port;
}

std::string Reimu::IPEndPoint::ToString(Reimu::IPEndPoint::ArgType t) {
	char sbuf[INET6_ADDRSTRLEN];

	if (t == String_IP) {
		inet_ntop(AddressFamily, Addr, sbuf, INET6_ADDRSTRLEN);
		return std::string(sbuf);
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
				     (struct sockaddr *)&SockAddr.v6 : (struct sockaddr *)&SockAddr.v4,
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
		return 0 == memcmp(&SockAddr.v6, &o.SockAddr.v6, sizeof(sockaddr_in6));
	} else {
		return 0 == memcmp(&SockAddr.v4, &o.SockAddr.v4, sizeof(sockaddr_in));
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
