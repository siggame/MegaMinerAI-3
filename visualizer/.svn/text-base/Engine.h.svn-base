//Ben Murrell
#pragma once

#include <SDL.h>
#include <SDL_opengl.h>
#include <GL/gl.h>

#include "Game.h"
#include <vector>
#include <string>
#include <map>
using std::vector;
using std::string;
using std::map;

#define PI (3.141592654f)
#define PI_OVER_3 (1.04719755f)
#define PI_OVER_6 (0.523598776f)

class Engine
{
	public:
        static Engine* instance();
		~Engine();

		//Engine stuff
		void init();
		void tick();
        void handleEvents();
		void handleKeys(SDL_Event* event);
		void handleMouseButtons(SDL_Event* event);
		void handleMouseMotion(SDL_Event* event);
		void windowResized(int width, int height);
		string lastError();

		//Drawing
		void drawHex(int hexX, int hexY);
		void drawFilledHex(GLfloat* center, const GLfloat& raySize);
		void drawHex(GLfloat* center, const GLfloat& raySize);
		void drawHexGrid(int minX, int maxX, int minY, int maxY);
		void drawHexCircle(int radius);

		void drawCircle(GLfloat* center, GLfloat radius);
		void drawDirectedCircle(GLfloat* center, GLfloat radius, GLfloat direction);
		void drawCrosshair(const GLfloat& x, const GLfloat& y, const GLfloat& size);

		//Display lists
		GLuint registerList(string list);
		GLuint recallList(string list);
		void   deleteList(string list);

		//Game Stuff
		void setGame(Game* game);
        void drawState(int stateNum, float percentThere); //If there are multiple games, they are run in lock-step
        void pollGameSource();
        void stopPolling();

		//Interpolation
		float interpolateOrientation(int current, int desired, const float& percentThere);

		//Unit Conversion
		static void hexToRect(float hexX, float hexY, GLfloat* rect);
		static float orientationToDegrees(float hexOrientation);


		//Shared state
		bool showHumanSight();
		bool showZombieSight();

	private:
        Engine();
		Game* m_game;
        SDL_Thread* m_readThread;

		//"Camera" info
		float m_zoom;
		float m_centerX;
		float m_centerY;

		int m_width;
		int m_height;

		//For arena
		bool m_exitAtEnd;
		unsigned int m_exitDelay;
		
		int m_curState;  //Current state #
		bool m_atEnd;
		bool m_skipForward;
		bool m_skipBackward;
		bool m_paused;

		bool m_showGrid; //Show grid?
		bool m_humanView;
		bool m_zombieView;

		unsigned int t_beginState; //time that the state began rendering (for animation)
        unsigned int t_lastCalled; //Last time tick was called
		unsigned int m_animTime; 

		GLfloat m_white[3];
		GLfloat m_black[3];


	private:
		map<string, GLuint> m_displayLists;

		//Display list names
		int m_L_hexCircleRadius;

		//Functions to create the display lists
		void createCircleList();
		void createDirectedCircleList();
		void createHexList();
		void createHexCircleList();
		void createCrosshairList();
		void createFilledHexList();
};
