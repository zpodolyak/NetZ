#ifndef SOCKET_ADDRESS_H
#define SOCKET_ADDRESS_H

class ClientAddress
{
public:
	ClientAddress();
	ClientAddress(const std::string &host_string);
	~ClientAddress() {}

	void SetPort(unsigned short port);
	int GetPort() const { return mIsResolved ? mAddress.sin_port : -1;
	void SetHostString(const std::string &host_string, unsigned short port=-1);
	std::string GetHostName() const { return mUnResolvedHostString; }	
	const sockaddr_in& GetAddress() const;
	void SetResolvedAddress(const sockaddr_in &addr);
private:
	bool  ResolveHost();
	
	std::string mUnresolvedHostString;
	bool mIsResolved;
	sockaddr_in mAddress;
};

#endif
