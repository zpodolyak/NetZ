#include "common.h"

void NetworkSocket::SetupNonBlocking()
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


void NetworkSocket::OnSocketEvent()
{

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
	return nbytes;	
}

int NetworkSocket::Write(const char *data, int length)
{
	int nbytes=0;
	if((nbytes=send(mSocket, data, length, 0))== -1)
	{
		SetError(SocketError::WRITE_ERROR_OCCURED,"send");
		return -1;
	}
	return nbytes;
}

int NetworkSocket::ReadTo(char *buffer, int buff_length)
{
	return -1;
}
int NetworkSocket::WriteTo(char *data, int length, ClientAddress &addr)
{
	return -1;
}

