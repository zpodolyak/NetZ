#ifndef S_SOCKET_H
#define S_SOCKET_H

struct SocketError
{
	enum eError
	{
		CONNECTION_ERROR,
		SOCKET_OPTIONS_FAILED,
		UNSPECIFIED_ERROR
	};

	eError mError;
	std::string mFunctionName;
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
	void Listen(unsigned short port);
	SSocket* Accept(ClientAddress &addr);
	void Close();
	void Connect(ClientAddress &addr);
	bool CreateSocket(int type, unsigned short);
	const SocketError& GetError() const { return mError; }
private:
	void SetupSocket();
	void SetError(SocketError::eError err, const char *error_text);
	SocketFD mSocket;
	SocketError mSocketError;
};

#endif
