#include "common.h"

SSocket::SSocket(SocketFD socket) : mSocket(socket) {}

SSocket::~SSocket() {}

bool SSocket::CreateSocket(int type, unsigned short port)
{
	struct addrinfo hints, *res, *p;
	int status;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC; 
	hints.ai_socktype = type;
	hints.ai_flags = AI_PASSIVE;
	
	if ((status = getaddrinfo(mUnresolvedHostString.c_str(), port, &hints, &res)) != 0)
	{	
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
		return false;
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
	if(p==NULL)
	{
		fprintf(stderr, "Failed to bind socket!\n");
		return false;
	}
	SetupSocket();
	return true;
}


void SSocket::SetError(SocketError::eError err, const char *fname)
{
	mSocketError.mError=err;
	mSocketError.mFunctionName=fname;
	mSocketError.mErrorText=strerror(errno);
	fprintf(stderr, "%s error: %s\n", mSocketError.mFunctionName.c_str(), mSocketError.mErrorText.c_str());
}

void SSocket::SetupSocket()
{

	int flags, s;
	flags = fcntl(mSocket, F_FGETL, 0);
	if(flags < 0)
	{
		SetError(SOCKET_OPTIONS_FAILED,"fcntl");
		return false;
	}

	flags |= O_NONBLOCK;
	s = fcntl(mSocket, F_SETFL, flags);
	if(f<0)
	{
		SetError(SOCKET_OPTIONS_FAILED,"fcntl");
		return false;
	}
}

void SSocket::Listen()
{
	listen(mSocket,10);	
}

SSocket* SSocket::Accept(ClientAddress &addr)
{
	SocketFD in;
	struct sockaddr_in client;
	socklen_t client_size = sizeof(sockaddr_in);

	in=accept(mSocket,(struct sockaddr*)&client, &client_size);
	
	if(in==-1)
	{
		SetError(CONNECTION_ERROR,"accept");
		return NULL;
	}

	addr.SetResolvedAddress(client);

	// TODO: maybe create normal client?
	SSocket *new_socket = new SSocket(in);
	return new_socket;
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
