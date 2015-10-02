#ifndef _WD_INETADDRESS_H
#define _WD_INETADDRESS_H

#include <unistd.h>
#include <arpa/inet.h>
#include <string>

class InetAddress
{
public:
	InetAddress();
	explicit InetAddress(uint16_t port);
	InetAddress(const std::string & ip, uint16_t port);
	InetAddress(const struct sockaddr_in &addr);
	void setSockAddrInet(struct sockaddr_in & addr);
	const struct sockaddr_in & getSockAddrInet()const;
	std::string ip() const;
	uint16_t port()const;
private:
	struct sockaddr_in addr_;
};
#endif
