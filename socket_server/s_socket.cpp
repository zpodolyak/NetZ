#include "common.h"

SSocket::SSocket(SocketFD socket) : mSocket(socket) {}

SSocket::~SSocket() {}

void SSocket::CreateStreamSocket()
{
	struct addrinfo hints, *res, *p;
	int status;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC; 
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	
	if ((status = getaddrinfo(mUnresolvedHostString.c_str(), SERVER_PORT_NUMBER, &hints, &res)) != 0) {
	{	
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
		return;
	}
	
	for(p=res; p!=NULL; p=p->ai_next)
	{
		mSocket = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
		if (listener < 0)  
		    continue;

		setsockopt(mSocket, SOL_SOCKET, SO_REUSEADDR, 1, sizeof(int));
	
	 	if (bind(mSocket, p->ai_addr, p->ai_addrlen) < 0) 
		{
		     	close(mSocket);
			continue;
		}
		break;
	}
	freeaddrinfo(res);
	// TODO: set error if p==NULL
}

void SSocket::CreateDatagramSocket()
{

}

SocketFD SSocket::Accept(ClientAddress &addr)
{

}

void SSocket::Connect(ClientAddress &addr)
{

}

int SSocket::Read(char *buffer, int buff_length)
{

}

int SSocket::Write(const char *data, int length)
{

}
int SSocket::ReadTo(char *buffer, int buff_length)
{

}
int SSocket::WriteTo(char *data, int length, ClientAddress &addr)
{

}

void SSocket::Close()
{
	close(mSocket);	
}
