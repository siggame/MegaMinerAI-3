//Copyright (C) 2009 by Ben Murrell
//Missouri S&T ACM AI

//READ ME!
//Current Dependencies: SDL, SDL_net
//You probably won't be able to build this unless you know what you're doing. When things are settled in, I'll commit a copy of the various libs.


#include <SDL.h>
#include <SDL_opengl.h>
#include <GL/gl.h>
#include <SDL_net.h>

#include <sstream>
#include <iostream>
#include <fstream>
using std::cout;
using std::cin;
using std::endl;
using std::ostringstream;
using std::ofstream;

#include "Config.h"
#include "GameObjectFactory.h"
#include "SExprSource.h"
#include "Socket.h"
#include "File.h"
#include "Buffer.h"
#include "GameState.h"
#include "Human.h"
#include "Crate.h"
#include "GameCoordinate.h"
#include "Engine.h"

//Function prototypes
string usage(char** argv);
bool verifyArgs(int argc, char** argv);

int main(int argc, char** argv)
{
	//Fill the config
	Config* config = Config::instance();
	config->init(argc, argv);
	if(config->get("c") != "")
	{
		config->readFile(config->configFile());
	}
	else
	{
		config->readFile("visualizer.cfg");
	}

	//Verify our inputs & give help
	if(!verifyArgs(argc, argv) || config->get("help") != "")
	{
		cout << usage(argv);
		return 0;
	}

	//Initialize the networking library
	if(SDLNet_Init() < 0)
	{
		cout << "SDLNet_Init: " << SDLNet_GetError() << endl;
		return 0;
	}


	//If we've made it this far, it's safe to assume there is a host XOR logfile
	SExprSource* sexprSource = 0;

	//Network mode
	if(config->get("h") != "")
	{
		//Create the socket we'll talk with
		Socket* server = new Socket();
		bool success;
	
		//Connect to the server
		cout << "Connecting... ";
		success = server->connect(config->get("h"));
		if(success)
		{
			cout << "success" << endl;
		}
		else
		{
			cout << "failed" << endl;
			return 0;
		}

		//Log into the server
		cout << "Logging in... ";
		success = server->login(config->user(), config->password());
		if(success)
		{
			cout << "success" << endl;	
		}
		else
		{
			//Login produces its own error message if it fails
			return 0;
		}

		if(config->get("g") != "")
		{
			server->writeSExpr(3, "join-game", config->get("g").c_str(), "spectator");
		}

		sexprSource = dynamic_cast<SExprSource*>(server);
	}

	//File mode
	else
	{
		File* file = new File();
		file->open(config->logFile());
		
		Buffer* buffer = new Buffer();
		buffer->readFile(file);

		sexprSource = dynamic_cast<SExprSource*>(buffer);
	}


	GameObjectFactory* objectFactory = GameObjectFactory::instance();
	objectFactory->registerHandler("Wall", new Wall);
	objectFactory->registerHandler("Human", new Human);
	objectFactory->registerHandler("Zombie", new Zombie);
	objectFactory->registerHandler("Crate", new Crate);
	objectFactory->registerHandler("Airstrike", new Airdrop);

	
	ofstream fout;
	fout.open( config->dumpFile().c_str() );

	//Main loop
    Engine* engine = Engine::instance();
	engine->init();
	
	Game* game = new Game();
	game->setSource(sexprSource);
	engine->setGame(game);

	while(true)
	{
		try
		{
			while(true)
			{
				engine->tick();
			}
		//Yes, this is a hack. You should probably never use exceptions like this. lol.
		}
		catch(...)
		{
			//If you're not operating on a socket and get a notify... well, you're screwed.
			Socket* socket = dynamic_cast<Socket*>(sexprSource);

			//Leave the game you're in & wait till it's ack'd
			socket->writeSExpr(1, "leave-game");
			while(socket->sexprReady(100))
			{
				SExpr result = socket->getSExpr();
				if(result[0] == "leave-accepted")
				{
					break;
				}
			}

			//Join the new game
			socket->writeSExpr(3, "join-game", game->nextGame().c_str(), "spectator");
			
			//Engine will clean up the old game for us
			game = new Game();
			game->setSource(sexprSource);
			engine->setGame(game);
		}
	}

	return 0;
}


//Output the usage for the program
string usage(char** argv)
{
	ostringstream sout;

	sout << "Usage: " << argv[0] << " [host game#] [options]" << endl;
	sout << "If no host is specified, a file must be specified in the options." << endl << endl;
	sout << "Options: " << endl;
	sout << "-f logfile : reads gamelog from the given logfile" << endl;
	sout << "-h hostname : connects to the given host for gamelog information" << endl;
	sout << "-g game number: joins the given game number as a spectator" << endl;
	sout << "-c configfile : uses the config in configfile rather than in visualizer.cfg" << endl;
	sout << "-d dumpfile : dumps gamelog information from the network into the given file" << endl;
	sout << "-help : displays this message" << endl;
	sout << "Note: command line options will override any conflicting options in a config file" << endl;

	return sout.str();
}


//Verify that the args we have received are what we want
bool verifyArgs(int argc, char** argv)
{
	Config* config = Config::instance();
	bool ret = true;
	
	//Read command line args into config
	config->init(argc, argv);

	bool hasFile = (config->logFile() != "");
	bool hasHost = (config->serverHostname() != "");

	//Make sure we have a file or a host
	if(!hasFile && !hasHost)
	{
		cout << "Error: Must specify host or log file as visualization source" << endl;
		ret = false;
	}

	//Make sure we don't have a file and a host
	if(hasFile && hasHost)
	{
		cout << "Error: Cannot specify host and log file as visualization source" << endl;
		ret = false;
	}

	if(hasFile && config->telnetMode())
	{
		cout << "Error: Cannot use telnet mode while reading from a logfile." << endl;
		ret = false;
	}
	
	if(hasHost && config->get("g")=="")
	{
		cout << "Warning: Host specified, but no game number specified" << endl;
		//ret = false;
	}

	if(ret == false)
	{
		cout << endl;
	}

	return ret;
}
