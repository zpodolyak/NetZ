#ifndef ADDRESS_H
#define ADDRESS_H

class Address
{
private:
	bool  ResolveHost();
	
	std::string mUnresolvedHostString;
	bool mIsResolved;
	sockaddr_in mAddress;
public:
	Address();
	Address(const std::string &host_string);
	~Address() {}

	void SetPort(unsigned short port);
	int GetPort() const { return mIsResolved ? mAddress.sin_port : -1; }
	void SetHostString(const std::string &host_string, unsigned short port=-1);
	std::string GetHostName() const { return mUnresolvedHostString; }	
	void SetResolvedAddress(const sockaddr_in &addr);
};

#endif
