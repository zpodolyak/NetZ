#include "common.h"

int main()
{
	Reactor theReactor;
	Netz::Socket server(theReactor, SOCK_STREAM, 3490);

	server.Listen();
	printf("waiting for connections...\n");
	
	for(;;)
	{
		theReactor.DispatchEvents();	
	}
	return 0;
}
