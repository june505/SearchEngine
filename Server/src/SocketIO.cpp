/*********************************
 *@fileName: SocketIO.cpp
 *@created : 2015-08-13 16:39:58
**********************************/

#include "../inc/SocketIO.h"
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>

#if 1
#include <iostream>
using namespace std;
#endif
ssize_t SocketIO::readn(char * buf,size_t count,InetAddress & clientAddr)
{
	size_t nleft = count;
	char * pbuf = buf;
	size_t nread;
	struct sockaddr_in clientaddr;
	size_t addrLen = sizeof(clientaddr);
	while(nleft > 0)
	{

		nread = ::recvfrom(sockfd_,pbuf,nleft,0,(struct sockaddr*)&clientaddr,&addrLen);
		if(-1 == nread)
		{
			if(errno == EINTR)
				continue;
			return -1;
		}
		else if(0 == nread)
		{
			break;
		}
		
		nleft -= nread;
		pbuf += nread;
	}
	clientAddr.setSockAddrInet(clientaddr);
	return (count - nleft);
}


ssize_t SocketIO::writen(char * buf,size_t count,InetAddress &clientAddr)
{
	size_t nleft = count;
	size_t nwrite;
	char * pbuf = buf;
	const struct sockaddr_in addr = clientAddr.getSockAddrInet();
	size_t addrLen = sizeof(addr);
	while(nleft > 0)
	{
		nwrite = ::sendto(sockfd_, buf, nleft,0,(struct sockaddr*)&addr,addrLen);
		if(-1 == nwrite)
		{
			if(errno == EINTR)
				continue;
			return -1;
		}

		nleft -= nwrite;
		pbuf += nwrite;
	}
	return (count - nleft);
}


ssize_t SocketIO::recv_peek(char *buf, size_t len)
{
	int nread;
	do
	{
		nread = ::recv(sockfd_, buf, len, MSG_PEEK);
	}while(-1 == nread  && errno == EINTR);
	return nread;
}

