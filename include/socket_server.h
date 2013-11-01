#ifndef SOCKET_SERVER_H
#define SOCKET_SERVER_H

class SocketServer
{
public:
	SocketServer(Reactor &reactor, IConnectionHandler *handler, int type, unsigned short port);

	void OnIncomingConnection();
	void Listen();
	NetworkSocket* Accept(ClientAddress &client);
private:
	int CreateServerSocket();
	
	Reactor &mReactor;
	int mServerType;
	unsigned short mServerPort;
	NetworkSocket mServerSocket;
	std::unique_ptr<IConnectionHandler> mConnectionHandler;
};

#endif
