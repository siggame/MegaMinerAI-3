#pragma once

#include "Map.h"
#include "GameObject.h"
#include "Animation.h"
#include "SExpr.h"

#include <deque>
using std::deque;

class GameState
{
	public:
		GameState();
		~GameState();
        GameState& operator=(const GameState& right);

		void fromSExpr(SExpr in);
		
		Map& map();
        deque<Animation*>& animations();
		deque<GameObject*>& dead();
        
        int curTurn();
		int humansReady();
		int humanHealth();
		float airstrikesReady();
		
		int numZombies();
		int zombiesReady();
		int zombieCap();

		void setCurTurn(int number);
		void setHumansReady(int humansReady);
		void setAirstrikesReady(float airstrikesReady);
		void setZombiesReady(int zombiesReady);
		void setZombieCap(int zombieCap);
	
	private:
		Map m_map;

		int m_curTurn;
		int m_humansReady;
		float m_airstrikesReady;
		int m_zombiesReady;
		int m_zombieCap;

		int m_humanHealth;
		int m_numZombies;

		deque<Animation*> m_animations;
		deque<GameObject*> m_dead;
};
