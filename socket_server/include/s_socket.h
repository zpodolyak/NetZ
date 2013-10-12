#ifndef S_SOCKET_H
#define S_SOCKET_H

struct SocketError
{
	enum eError
	{
		CONNECTION_ERROR,
		SOCKET_OPTIONS_FAILED,
		REMOTE_HOST_CLOSED_CONNECTION,
		READ_ERROR_OCCURED,
		WRITE_ERROR_OCCURED,
		UNSPECIFIED_ERROR
	};

	eError mError;
	std::string mFunctionName;
	std::string mErrorText;

	SocketError() : mError(UNSPECIFIED_ERROR) {}
};

typedef int SocketFD;
class ClientAddress;

class NetworkSocket
{
public:
	NetworkSocket(){}
	NetworkSocket(SocketFD fd) : mSocket(fd) {}
	~NetworkSocket(){}
	
	int Read(char *buffer, int buff_length);
	int Write(const char *data, int length);
	int ReadTo(char *buffer, int buff_length);
	int WriteTo(char *data, int length, ClientAddress &addr);
	void Listen();
	NetworkSocket* Accept();
	bool CreateSocket(int type, unsigned short port);
	void Connect(ClientAddress &address);
	
	void Close()
	{
		close(mSocket);
	}
	const SocketError& GetError() const { return mSocketError; }
private:
	void SetupSocket();
	
	void SetError(SocketError::eError err, const char *error_text)
	{
		mSocketError.mError=err;
		mSocketError.mFunctionName=error_text;
		mSocketError.mErrorText=strerror(errno);
		fprintf(stderr, "%s error: %s\n", mSocketError.mFunctionName.c_str(), mSocketError.mErrorText.c_str());
	}

	SocketFD mSocket;
	SocketError mSocketError;
};

#endif
