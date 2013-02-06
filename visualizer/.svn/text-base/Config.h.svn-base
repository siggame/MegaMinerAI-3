//Ben Murrell
#pragma once

#include <map>
#include <string>
using std::map;
using std::string;

//Class to hold config for the visualizer
class Config
{
	public:
		~Config();
		static Config* instance();
		void init(int argc, char** argv);
		void readFile(string path);

		string get(string key);
		void set(string key, string value);

		//Visualizer Specific
		string user();
		string password();

		string configFile();
		string logFile();
		string serverHostname();
		string dumpFile();


		bool   fullscreen();
		bool   telnetMode();
		bool   animate();

		unsigned int animTime();

	private:
		Config();
		map<string, string> m_attributes;

};
