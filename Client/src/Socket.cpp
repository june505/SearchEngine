/*********************************
 *@fileName: Socket.cpp
 *@created : 2015-08-13 16:06:33
**********************************/

#include "../inc/InetAddress.h"
#include "../inc/Socket.h"

#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>


Socket::Socket()
{
	sockfd_ = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(sockfd_ == -1)
	{
		perror("socket error");
		exit(EXIT_FAILURE);
	}
}

const int Socket::fd()const
{
	return sockfd_;
}

void Socket::bind(InetAddress & addr)
{
	if(-1 == ::bind(sockfd_, 
		   (struct sockaddr*)addr.getSockAddrInet(), 
			sizeof(struct sockaddr)))
	{
		perror("bind error");
		exit(EXIT_FAILURE);
	}
}

#if 0
void Socket::listen()
{
	if(-1 == ::listen(sockfd_, 10))
	{
		perror("listen error");
		exit(EXIT_FAILURE);
	}
}

#endif

#if 0 
int Socket::accept()
{
	int peerfd = ::accept(sockfd_, NULL, NULL);
	if(-1 == peerfd)
	{
		perror("accept error");
		exit(EXIT_FAILURE);
	}
	InetAddress local = localAddress(peerfd);
	InetAddress peer  = peerAddress(peerfd);
	printf("connected! %s:%d -> %s:%d\n", 
			local.ip().c_str(),
			local.port(),
			peer.ip().c_str(),
			peer.port());

	return peerfd;
}
#endif

void Socket::setAddrReuse(bool on)
{
	int optval = on ? 1 : 0;
	if(-1 ==::setsockopt(sockfd_, 
			SOL_SOCKET,
			SO_REUSEADDR,
			&optval,
			static_cast<socklen_t>(sizeof(optval))))
	{
		perror("setsockopt addrReuse error");
		exit(EXIT_FAILURE);
	}
}

void Socket::setPortReuse(bool on)
{
#ifdef SO_REUSEPORT
	int optval = on ? 1 : 0;
	if(-1 == ::setsockopt(sockfd_, 
			SOL_SOCKET,
			SO_REUSEPORT,
			&optval,
			static_cast<socklen_t>(sizeof(optval))))
	{
		perror("setsockopt portReuse errror");
	}

#else
	if(on)
	{
		perror("SO_REUSEPORT is not supported.\n");
	}

#endif
}


void Socket::ready(InetAddress & addr)
{
	setPortReuse(true);
	bind(addr);
	//listen();
}

InetAddress Socket::localAddress(int sockfd)
{
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	socklen_t len = sizeof(addr);
	if(-1 == ::getsockname(sockfd, 
			(struct sockaddr*)&addr, 
			&len))
	{
		perror("localAddress error");
	}
	return InetAddress(addr);
}

InetAddress Socket::peerAddress(int sockfd)
{
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	socklen_t len = sizeof(addr);
	if(-1 == ::getpeername(sockfd, 
			(struct sockaddr*)&addr, 
			&len))
	{
		perror("peerAddress error");
	}
	return InetAddress(addr);
}
