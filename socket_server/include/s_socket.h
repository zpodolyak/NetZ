#ifndef S_SERVER_H
#define S_SERVER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

struct SocketError
{

};

typedef int SocketFD;

class ClientAddress;

class SSocket
{
public:
	SSocket(SocketFD fd);
	virtual ~SSocket();

	virtual int Read(char *buffer, int buff_length);
	virtual int Write(const char *data, int length);
	virtual int ReadTo(char *buffer, int buff_length);
	virtual int WriteTo(char *data, int length, ClientAddress &addr);
	virtual void Bind(int port);
	virtual void Listen(int port);
	virtual SocketFD Accept(ClientAddress &addr);
	virtual void Close();
	virtual void Connect(ClientAddress &addr);
	virtual void CreateStreamSocket();
	virtual void CreateDatagramSocket();
	virtual SocketError& GetError() const { return mError; }
private:
	SocketFD mSocket;
	SocketError mError;
};

#endif
