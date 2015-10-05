CC = g++
TARGET = ../bin/client
INC = ../inc/*.h
OBJ = udp_client.o Socket.o SocketIO.o InetAddress.o
$(TARGET):$(OBJ)
	$(CC) -o $(TARGET) $(OBJ)

udp_client.o:udp_client.cpp $(INC)
	$(CC) -c udp_client.cpp

Socket.o:Socket.cpp ../inc/Socket.h ../inc/InetAddress.h
	$(CC) -c Socket.cpp

SocketIO.o:SocketIO.cpp ../inc/SocketIO.h
	$(CC) -c SocketIO.cpp

InetAddress.o:InetAddress.cpp ../inc/InetAddress.h
	$(CC) -c InetAddress.cpp

.PHONY:clean
clean:
	rm -f $(OBJ)


