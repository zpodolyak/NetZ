#include "common.h"

int main()
{
	NetworkSocket socket;
	if(socket.CreateSocket(SOCK_STREAM,3490))
	{
		socket.Listen();
		printf("Someone should connect to mah shit\n");
		NetworkSocket *newfd=socket.Accept();
		delete newfd;
	}
	return 0;
}
