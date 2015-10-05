#ifndef _WD_SOCKET_H
#define _WD_SOCKET_H

class InetAddress;
class Socket
{
public:
	Socket();
	const int fd() const;
	void bind(InetAddress & addr);
	//void listen();
	//int accept();
	void setAddrReuse(bool);
	void setPortReuse(bool);
	void ready(InetAddress & addr);
	static InetAddress localAddress(int);
	static InetAddress peerAddress(int);
private:
	int sockfd_;
};

#endif

