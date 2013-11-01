#ifndef GAME_CONNECTION_H
#define GAME_CONNECTION_H

class GameConnection : public IConnectionHandler
{
public:
	virtual ~GameConnection(){}

	virtual void HandleConnection(const ClientAddress &client, NetworkSocket *incoming)
	{
		char s[INET6_ADDRSTRLEN];
		const sockaddr_in &c = client.GetClientAddress();

		inet_ntop(c.sin_family, &c.sin_addr, s, sizeof(s));
		printf("server: got connection from %s\n", s);

		const char *reply = "My epoll shit just answered like a capo di tutti capi!";
		size_t len = strlen(reply);
		incoming->Write(reply, len);
		incoming->Close();
		delete incoming;
	}
};

#endif
