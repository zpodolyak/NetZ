#include "common.h"

int main()
{
	Reactor theReactor;
	SocketServer server(theReactor, new GameConnection, SOCK_STREAM, 3490);

	server.Listen();
	printf("Someone should connect to mah shit\n");
	
	for(;;)
	{
		theReactor.DispatchEvents();	
	}
	return 0;
}
