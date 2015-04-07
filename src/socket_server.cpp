#include "common.h"

SocketServer::SocketServer(Reactor &reactor, IConnectionHandler *handler,  int type, unsigned short port)
: mReactor(reactor)
, mServerType(type)
, mServerPort(port)	
, mServerSocket(CreateServerSocket())
{
	mConnectionHandler.reset(handler);
	mServerSocket.SetupNonBlocking();
	mReactor.AddEventHandler(new Event(mServerSocket.GetFD()), std::bind(&SocketServer::OnIncomingConnection,this));
}

void SocketServer::OnIncomingConnection()
{
	ClientAddress client;
	if(NetworkSocket *s=Accept(client))
		mConnectionHandler->HandleConnection(client,s);
}

void SocketServer::Listen()
{
	listen(mServerSocket.GetFD(),10);	
}

NetworkSocket* SocketServer::Accept(ClientAddress &address)
{
	int in;
	struct sockaddr_in client;
	socklen_t client_size = sizeof(sockaddr_in);

	in=accept(mServerSocket.GetFD(),(struct sockaddr*)&client, &client_size);
	
	if(in==-1)
	{
		mServerSocket.SetError(SocketError::CONNECTION_ERROR,"accept");
		return NULL;
	}
	
	address.SetResolvedAddress(client);
	NetworkSocket *newSocket=new NetworkSocket(in);
	newSocket->SetupNonBlocking();
	mReactor.AddEventHandler(new Event(in), std::bind(&NetworkSocket::OnSocketEvent,newSocket));
	return newSocket;
}

int SocketServer::CreateServerSocket()
{
	struct addrinfo hints, *res, *p;
	int status, sfd;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC; 
	hints.ai_socktype = mServerType;
	hints.ai_flags = AI_PASSIVE;
	
	if ((status = getaddrinfo(NULL, to_string(mServerPort).c_str(), &hints, &res)) != 0)
	{	
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
		return false;
	}
	
	for(p=res; p!=NULL; p=p->ai_next)
	{
		sfd  = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
		if (sfd < 0)  
		    continue;
		
		int yes=1;
		setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
	
	 	if (bind(sfd, p->ai_addr, p->ai_addrlen) < 0) 
		{
		     	close(sfd);
			continue;
		}
		break;
	}
	freeaddrinfo(res);
	if(p==NULL)
	{
		fprintf(stderr, "Failed to bind socket!\n");
		return -1;
	}
	return sfd;
}
