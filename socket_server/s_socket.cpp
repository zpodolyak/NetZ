#include "common.h"

bool NetworkSocket::CreateSocket(int type, unsigned short port)
{
	struct addrinfo hints, *res, *p;
	int status;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC; 
	hints.ai_socktype = type;
	hints.ai_flags = AI_PASSIVE;
	
	if ((status = getaddrinfo(NULL, to_string(port).c_str(), &hints, &res)) != 0)
	{	
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
		return false;
	}
	
	for(p=res; p!=NULL; p=p->ai_next)
	{
		mSocket = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
		if (mSocket < 0)  
		    continue;
		
		int yes=1;
		setsockopt(mSocket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
	
	 	if (bind(mSocket, p->ai_addr, p->ai_addrlen) < 0) 
		{
		     	close(mSocket);
			continue;
		}
		break;
	}
	freeaddrinfo(res);
	if(p==NULL)
	{
		fprintf(stderr, "Failed to bind socket!\n");
		return false;
	}
	//SetupSocket();
	return true;
}

void NetworkSocket::SetupSocket()
{

	int flags, s;
	flags = fcntl(mSocket, F_GETFL, 0);
	if(flags < 0)
	{
		SetError(SocketError::SOCKET_OPTIONS_FAILED,"fcntl");
		return;
	}

	flags |= O_NONBLOCK;
	s = fcntl(mSocket, F_SETFL, flags);
	if(s<0)
	{
		SetError(SocketError::SOCKET_OPTIONS_FAILED,"fcntl");
		return;
	}
}

void NetworkSocket::Listen()
{
	listen(mSocket,10);	
}

NetworkSocket* NetworkSocket::Accept()
{
	SocketFD in;
	struct sockaddr_in client;
	socklen_t client_size = sizeof(sockaddr_in);

	in=accept(mSocket,(struct sockaddr*)&client, &client_size);
	
	if(in==-1)
	{
		SetError(SocketError::CONNECTION_ERROR,"accept");
		return NULL;
	}

	NetworkSocket *new_socket = new NetworkSocket(in);
	return new_socket;
}

int NetworkSocket::Read(char *buffer, int buff_length)
{
	int nbytes=recv(mSocket, buffer, buff_length,0);

	if(nbytes==-1)
	{
		SetError(SocketError::READ_ERROR_OCCURED,"recv");
		return -1;
	}
	else if(nbytes==0)
	{
		fprintf(stderr, "Remote host closed the connection.\n");
		return 0;
	}	
}

int NetworkSocket::Write(const char *data, int length)
{
	int nbytes=send(mSocket, data, length, 0);

}

int NetworkSocket::ReadTo(char *buffer, int buff_length)
{

}
int NetworkSocket::WriteTo(char *data, int length, ClientAddress &addr)
{

}

