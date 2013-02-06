//Ben Murrell
#include "Config.h"

#include <stdlib.h>
#include <fstream>
#include <iostream>
using std::ifstream;
using std::cerr;
using std::endl;

Config::Config()
{
	m_attributes["animate"] = "1";
}

Config* Config::instance()
{
	static Config* inst = new Config();
	return inst;
}

Config::~Config()
{


}

// -fullscreen -f lol.txt -r -m
void Config::init(int argc, char** argv)
{
	for(int i=1; i < argc; ++i)
	{
		//-
		if(argv[i][0] == '-')
		{
			//-h -c -s -fullscreen (the last switch)
			if((i+1) >= argc)
			{
				string attr = argv[i];
				m_attributes[attr.substr(1)] = "1";
			}

			//-h -c -s
			else if(argv[i+1][0] == '-')
			{
				string attr = argv[i];
				m_attributes[attr.substr(1)] = "1";
			}

			//-f lol.txt
			else
			{
				string attr = argv[i];
				m_attributes[attr.substr(1)] = string(argv[i+1]);
				++i;
			}
		}

		//We'll take the first parameter as the hostname
		else if(i == 1)
		{
			m_attributes["h"] = string(argv[i]);
		}

		//We'll take the second param as the game #
		else if(i == 2)
		{
			m_attributes["g"] = string(argv[i]);
		}
	}


}

void Config::readFile(string path)
{
	ifstream fin;
	fin.open(path.c_str());

	if(fin.fail())
	{
		//Error!
		cerr << "Error reading config file: " << path << endl;
		return;
	}

	string buf;
	while(!fin.eof())
	{
		getline(fin, buf, '\n');
		size_t splitAt = buf.find('=');

		//Make sure there was an =
		if(splitAt == string::npos)
		{
			continue;
		}
		
		//Find the end of the key (cut out whitespace)
		size_t keyEnd = splitAt;
		do
		{
			keyEnd--;
		} while(keyEnd > 0 && buf[keyEnd] == ' ');

		//Find the beginning of the value (cut out whitespace)
		size_t valueBegin = splitAt;
		do
		{
			valueBegin++;
		} while(valueBegin < buf.size() && buf[valueBegin] == ' ');

		//Put the key,value pair into our map
		m_attributes[buf.substr(0, keyEnd+1)] = buf.substr(valueBegin); 
	}
	

}

string Config::get(string key)
{
	return m_attributes[key];
}

void Config::set(string key, string value)
{
	m_attributes[key] = value;
}

//Visualizer Specific
string Config::user()
{
	return m_attributes["user"];
}

string Config::password()
{
	return m_attributes["password"];
}

bool Config::fullscreen()
{
	return atoi(m_attributes["fullscreen"].c_str()) != 0;
}

bool Config::animate()
{
	return atoi(m_attributes["animate"].c_str()) != 0;
}

string Config::configFile()
{
	return m_attributes["c"];
}

string Config::logFile()
{
	return m_attributes["f"];
}

string Config::serverHostname()
{
	return m_attributes["h"];
}

bool Config::telnetMode()
{
	return atoi(m_attributes["t"].c_str()) != 0;
}

string Config::dumpFile()
{
	return m_attributes["d"];
}

unsigned int Config::animTime()
{
	if(m_attributes["animTime"] != "")
	{
		return atoi(m_attributes["animTime"].c_str());
	}

	//Default to 1000
	return 1000;
}
