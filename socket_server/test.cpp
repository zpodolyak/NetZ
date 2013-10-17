#include "common.h"

int main()
{
	NetworkSocket socket(false);
	ClientAddress client;
	char s[INET6_ADDRSTRLEN];
	if(socket.CreateSocket(SOCK_STREAM,3490))
	{
		socket.Listen();
		printf("Someone should connect to mah shit\n");
		
		for(;;)
		{
			std::auto_ptr<NetworkSocket> newfd(socket.Accept(client));
			if(!newfd.get())
			{
				perror("accept");
				continue;
			}
			
			const sockaddr_in &c = client.GetClientAddress();

			inet_ntop(c.sin_family, &c.sin_addr, s, sizeof(s));
			printf("server: got connection from %s\n", s);

			const char *reply = "My Ubuntu shit just answered like a boss!";
			size_t len = strlen(reply);
			newfd->Write(reply, len);
			newfd->Close();
		}
	}
	return 0;
}
