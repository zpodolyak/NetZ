#ifndef S_SERVER_H
#define S_SERVER_H

struct SocketError
{
	enum eError
	{
		CONNECTION_ERROR,
		UNSPECIFIED_ERROR
	};

	eError mError;
	std::string mErrorText;

	SocketError() : mError(UNSPECIFIED_ERROR) {}
};

typedef int SocketFD;

class ClientAddress;

class SSocket
{
public:
	SSocket(SocketFD fd);
	~SSocket();
	
	int Read(char *buffer, int buff_length);
	int Write(const char *data, int length);
	int ReadTo(char *buffer, int buff_length);
	int WriteTo(char *data, int length, ClientAddress &addr);
	void Bind(int port);
	void Listen(int port);
	SocketFD Accept(ClientAddress &addr);
	void Close();
	void Connect(ClientAddress &addr);
	void CreateStreamSocket();
	void CreateDatagramSocket();
	const SocketError& GetError() const { return mError; }
private:
	SocketFD mSocket;
	SocketError mError;
};

#endif
