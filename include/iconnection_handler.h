#ifndef ICONNECTION_HANDLER_H
#define ICONNECTION_HANDLER_H

class IConnectionHandler
{
public:
	virtual ~IConnectionHandler(){}

	virtual void HandleConnection(const ClientAddress &client, NetworkSocket *incoming)=0;
};

#endif
