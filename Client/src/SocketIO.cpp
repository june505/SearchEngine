#include "../inc/SocketIO.h"
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>

ssize_t SocketIO::readn(char * buf,size_t count,InetAddress & clientAddr)
{
	size_t nleft = count;
	char * pbuf = buf;
	size_t nread;
	size_t addrLen = sizeof(clientAddr);
	while(nleft > 0)
	{

		nread = ::recvfrom(sockfd_,pbuf,nleft,0,(struct sockaddr*)&clientAddr,&addrLen);
		//nread = ::read(sockfd_, pbuf, nleft);
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
	return (count - nleft);
}


ssize_t SocketIO::writen(char * buf,size_t count,InetAddress &clientAddr)
{
	size_t nleft = count;
	size_t nwrite;
	char * pbuf = buf;
	size_t addrLen = sizeof(clientAddr);
	while(nleft > 0)
	{
		nwrite = ::sendto(sockfd_, buf, nleft,0,(struct sockaddr*)&clientAddr,addrLen);
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

#if 0
ssize_t SocketIO::readline(char * buf, size_t maxlen)
{
	size_t nleft = maxlen - 1;
	char * pbuf = buf;
	ssize_t nread;
	size_t total = 0;

	while(nleft > 0)
	{
		nread = recv_peek(pbuf, nleft);
		if(nread <= 0)
			return nread;
		int i;
		for(i = 0; i != nread; ++i)
		{
			 //当找到'\n'
			if(pbuf[i] == '\n')
			{
				size_t nsize = i + 1;
				if(static_cast<ssize_t>(nsize) 
						!= readn(pbuf, nsize))
					return -1;
				pbuf += nsize;
				total += nsize;
				*pbuf = 0;
				return total;
			}
		}

		//当没有找到'\n'
		if(readn(pbuf, nread) != nread)
			return -1;
		total += nread;
		pbuf += nread;
		nleft -= nread;
	}
	*pbuf = 0;
	return maxlen - 1;
}
#endif
