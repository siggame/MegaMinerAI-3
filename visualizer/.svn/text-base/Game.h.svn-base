//Ben Murrell
#pragma once

#include "GameState.h"
#include "SExprSource.h"

#include <SDL.h>
#include <SDL_opengl.h>
#include <GL/gl.h>
#include <SDL_mutex.h>

#include <map>
#include <deque>
using std::deque;
using std::map;

class Game
{
	public:
		Game();
		~Game();

        //Set the source of messages
		void setSource(SExprSource* source);
		bool pollSource();

		//Furthest coordinate an object has been at
		GameCoordinate furthest();
		GameCoordinate minCoord();
		GameCoordinate maxCoord();

		//number of states
		int size();

		//Last state accessed
		int prevTurn();
		GameState* operator[](int index);
		
		//Add state to end of game
		void pushState(GameState* state);

		//Names of the AI's in the game
		string zombieName();
		string humanName();

		void reset();
		string nextGame();

	private:
        SDL_mutex* m_stateLock;

		deque<GameState*> m_states;
		map<int, Wall*> m_curWalls;

        int m_prevTurn;
		string m_humanName;
		string m_zombieName;

		//deque<Weapon> m_weapons;
		SExprSource* m_sexprSource;
		string m_nextGame;

		GameCoordinate m_maxCoord;
		GameCoordinate m_minCoord;

        void splitState(GameState* state);
        void updateBoundary(GameState* state);
        void updateWalls(GameState* state);
        void stripState(GameState* state);
};
