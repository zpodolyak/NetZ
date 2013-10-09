include "common.h"

ClientAddress::ClientAddress()
: mIsResolved(false)
{
	memset(&mAddress,0,sizeof(sockaddr_in));
}

ClientAddress::ClientAddress(const std::string &host_string)
: mUnresolvedHostString(host_string)
, mIsResolved(false)
{
	memset(&mAddress,0,sizeof(sockaddr_in));
	mIsResolved=ResolveHost();
}

void ClientAddress::SetPort(unsigned short port)
{
	mAddress.sin_port=htons(port);
}

void ClientAddress::SetHostString(const std::string &host_string, unsigned short port)
{
	mUnresolvedHostString=host_string;
	if(port != -1)
		SetPort(port);
	mIsResolved=ResolveHost();
}

bool ClientAddress::ResolveHost()
{
	std::string::size_type found=mUnresolvedHostString.find_first_of(":");
	std::string port;

	if(found!=std::string::npos && inet_pton(AF_INET,host_string.substr(0,found),&(mAddress.sin_addr)) > 0)
	{
		SetPort(atoi(host_string.substr(found).c_str()));
		port=host_string.substr(found);	
		return true;
	}

	struct addrinfo hints, *res, *p;
	int status;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC; 
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_CANONNAME;
	port=to_string(mAddress.sin_port);

	if ((status = getaddrinfo(mUnresolvedHostString.c_str(), port.c_str(), &hints, &res)) != 0) {
	{	
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
		return false;
	}
	
	for(p=res; p!=NULL; p=p->ai_next)
	{
		if(p->ai_family!=AF_INET)
			continue;
		mAddress.sin_addr=((const struct sock_addr_in*)p->ai_addr)->sin_addr;
		break;
	}

	freeaddrinfo(res);
	return mAddress.sin_addr != 0;
}

void ClientAddress::SetResolvedAddress(const sockaddr_in &addr)
{
	mAddress=addr;
	mIsResolved=true;
}
