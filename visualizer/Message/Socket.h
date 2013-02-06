//Copyright (C) 2009 by Ben Murrell
#pragma once

#include "SDL_net.h"

#include <string>
using std::string;

#include "SExpr.h"
#include "SExprSource.h"

namespace SocketState 
{
	enum {None, Connecting, Connected, Disconnected, Failed};
}

/*  \brief This class represents a network socket that can be used to send/receive data
 *         via Steven Wallace's server protocol. It also acts as a source of s-expressions.
 *         It acts as an object oriented wrapper to the SDL_Net C library.
 */
class Socket : public SExprSource
{
	public:
		Socket();
		virtual ~Socket();

		//Connect / Disconnect
		bool connect(string host, int port = 19000);
		void disconnect();

		//Low level
		bool send(const char* data, int size);
		bool send(string data);
		bool dataReady(int msWait = 0);
		string receive();

		//Medium level
		bool sendSExpr(int n, ...);
		SExpr receiveSExpr();

		//Higher level
		bool login(string user, string pass);
		
		//Status
		unsigned int state();

		//SExprSource functions
		virtual bool sexprReady(int msWait);
		virtual SExpr getSExpr();
		virtual bool endOfSource();
		virtual bool write(string toWrite);

	private:
		IPaddress m_ip;
		TCPsocket m_sd;
		SDLNet_SocketSet m_set;

		unsigned int m_state;

		void disconnected();
};

