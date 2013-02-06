//Copyright (C) 2009 by Ben Murrell
#include "Socket.h"

#include <algorithm>
#include <iostream>
using std::cout;
using std::endl;
using std::min;

Socket::Socket()
{
	//So we can use select
	m_set = SDLNet_AllocSocketSet(1);
}

Socket::~Socket()
{
	SDLNet_FreeSocketSet(m_set);
}

//Connect
bool Socket::connect(string host, int port)
{
	if(m_state == SocketState::Connected || m_state == SocketState::Connecting)
	{
		cout << "Socket already connected!" << endl;
		return false;
	}

	m_state = SocketState::Connecting;

	if(SDLNet_ResolveHost(&m_ip, host.c_str(), port) < 0)
	{
		cout << "SDLNet_ResolveHost: " << SDLNet_GetError() << endl;
		m_state = SocketState::Failed;
		return false;
	}

	if(!(m_sd = SDLNet_TCP_Open(&m_ip)))
	{
		cout << "SDLNet_TCP_Open: " << SDLNet_GetError() << endl;
		m_state = SocketState::Failed;
		return false;
	}

	//Add the socket to our socket set so we can select()
	SDLNet_AddSocket(m_set, (SDLNet_GenericSocket)m_sd);

	//Receive the first string from server ("nzbn")
	if(this->receive().empty())
	{
		cout << "Failed to receive the initial handshake from server" << endl;
		m_state = SocketState::Failed;
		return false;
	}

	//Tell the server we don't use compression ("n")
	if(!this->send("n"))
	{
		cout << "Failed to respond to handshake" << endl;
		m_state = SocketState::Failed;
		return false;
	}


	m_state = SocketState::Connected;
	return true;
}


//Disconnect
void Socket::disconnect()
{
	if(m_state == SocketState::Connected || m_state == SocketState::Connecting)
	{
		SDLNet_TCP_Close(m_sd);
	}

	disconnected();
}


//Send as the server expects to receive
bool Socket::send(const char* data, int size)
{
	//Send size
	int totalSize = 1 + size;
	totalSize = SDLNet_Write32(totalSize, (void*)&totalSize);//  htonl(totalSize);
	if(SDLNet_TCP_Send(m_sd, (void*)&totalSize, sizeof(totalSize)) < sizeof(totalSize))
	{
		cout << "SDLNet_TCP_Send: " << SDLNet_GetError() << " (Sending size of message)" << endl;
		disconnected();	
		return false;
	}

	//Send "n" for no compression
	char compression[] = "n";
	if(SDLNet_TCP_Send(m_sd, (void*)compression, 1) < 1)
	{
		cout << "SDLNet_TCP_Send: " << SDLNet_GetError() << " (Sending compression)" << endl;
		disconnected();
		return false;
	}

	//Send payload
	if(SDLNet_TCP_Send(m_sd, (void*)data, size) < size)
	{
		cout << "SDLNet_TCP_Send: " << SDLNet_GetError() << " (Sending message payload)" << endl;
		disconnected();
		return false;
	}
	return true;
}


//Make sending easier
bool Socket::send(string data)
{
	return send(data.c_str(), data.size());
}


//Retrieve one message
string Socket::receive()
{
	//Get the payload size (4 bytes) and convert it to our byte order
	int totalSize;
	if(SDLNet_TCP_Recv(m_sd, (void*)&totalSize, 4) < 4)
	{
		cout << "SDLNet_TCP_Recv: " << SDLNet_GetError() << "(Receiving payload size)" << endl;
		disconnected();
		return "";
	}
	totalSize = SDLNet_Read32((void*)&totalSize);

	char readBuffer[1024];
	string result;

	//Receive the whole message
	while(totalSize > 0)
	{
		int toRead = min(1024-1, totalSize);
		int sizeRead = SDLNet_TCP_Recv(m_sd, (void*)readBuffer, toRead);
		if(sizeRead == 0)
		{
			cout << "SDLNet_TCP_Recv: " << SDLNet_GetError() << " (Receiving payload)" << endl;
			disconnected();
			return "";
		}

		totalSize -= sizeRead;
		readBuffer[sizeRead] = '\0';
		result += readBuffer;
	}

	//Verify it isn't compressed
	if(result.size() > 0 && result[0] != 'n')
	{
		cout << "Unsupported compression algorithm: " << result[0] << endl;
		return "";
	}

	//Chop off the prepended "n" for no compression
	result = result.substr(1);

	//Return the final result
	//cout << "Size: " << result.size() << endl;
	//cout << "socket recv'd: " << result << endl;
	return result;
}

//Return the state of the socket
unsigned int Socket::state()
{
	return m_state;
}

//Log into the server
bool Socket::login(string user, string pass)
{
	if(!sendSExpr(3, "login", user.c_str(), pass.c_str()))
	{
		cout << "Failed to send login message" << endl;
		return false;
	}

	SExpr success = receiveSExpr();

	if(success[0] == "login-denied")
	{
		cout << "Login denied, reason: " << success[1] << endl;
		return false;
	}
	
	return true;
}


//Call when disconnected
void Socket::disconnected()
{
	m_state = SocketState::Disconnected;
	cout << "Disconnected" << endl;
}


bool Socket::sendSExpr(int n, ...)
{
	//Get the ...'s
	va_list argp;
	va_start(argp, n);
	
	string toSend = "(";
	for(int i=0; i < (n-1); ++i)
	{
		toSend += "\"" + string(va_arg(argp, char*)) + "\"";
	}
	toSend += "\"" + string(va_arg(argp, char*)) + "\")";
	va_end(argp);

	return send(toSend);
}

SExpr Socket::receiveSExpr()
{
	string message = receive();
	return SExpr(message);
}

bool Socket::dataReady(int msWait)
{
	if(m_state != SocketState::Connected)
	{
		return false;
	}

	int result = SDLNet_CheckSockets(m_set, msWait); 
	
	if(result == -1)
	{
		cout << "select() failed" << endl;
	}
	return (result > 0);
}


bool Socket::sexprReady(int msWait)
{
	return dataReady(msWait);
}

SExpr Socket::getSExpr()
{
	return receiveSExpr();
}

bool Socket::endOfSource()
{
	bool ret = false;

	if(m_state == SocketState::Disconnected || m_state == SocketState::Failed || m_state == SocketState::None)
	{
		ret = true;
	}

	return ret;
}

bool Socket::write(string toWrite)
{
	return this->send(toWrite);
}
