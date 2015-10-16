#include "../inc/InetAddress.h"
#include "../inc/Socket.h"
#include "../inc/SocketIO.h"

#include <iostream>
#include <sys/select.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define IP "127.0.0.1"
#define PORT 8888
#define MAX(a,b) (a>b?a:b)

using namespace std;
int main()
{
	InetAddress servAddr(IP,PORT);
	size_t addrLen = sizeof(servAddr);
	Socket socket;
	int nready;
	int clientfd = socket.fd();
	char buf[128];
	char *res = new char[1024*1024];
	fd_set readset;
	cout<<"客户端已经就绪>>>"<<endl;
	while(1)
	{
		FD_ZERO(&readset);
		FD_SET(clientfd,&readset);
		FD_SET(STDIN_FILENO,&readset);
		int maxfd=MAX(clientfd,STDIN_FILENO);
		memset(buf,'\0',sizeof(buf));
		memset(res,'\0',1024*1024);
		nready = select(maxfd+1,&readset,NULL,NULL,NULL);
		if(nready == -1)
		{
			perror("selcet error!");
			exit(1);
		}
		else if(nready == 0)
		{
			continue;
		}

		if(FD_ISSET(clientfd,&readset))
		{
	
			int ret = recvfrom(clientfd,res,sizeof(res),0,(struct sockaddr*)&servAddr,&addrLen);
			if(ret<0)
			{
				perror("recvfrom error!");
				exit(1);
			}
			cout<<"strlen(res):"<<strlen(res)<<res<<endl<<"--------"<<endl;
			
		}
		if(FD_ISSET(STDIN_FILENO,&readset))
		{
			int n = read(STDIN_FILENO,buf,sizeof(buf));
			if(n<0)
			{
				perror("read error!");
				exit(1);
			}
			 sendto(clientfd,buf,sizeof(buf),0,(struct sockaddr*)&servAddr,sizeof(servAddr));
		}
		
	}

	return 0;
}
