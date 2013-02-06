#pragma once

#include "SExpr.h"

#include "GameCoordinate.h"
#include "GameObject.h"
#include "Human.h"
#include "Zombie.h"
#include "Wall.h"
#include "Airdrop.h"

#include <map>
using std::map;

class Map
{
	public:
		Map();
		~Map();
        Map& operator=(const Map& right);

		GameCoordinate& max();
		GameCoordinate& min();

		void addObject(GameObject* object);		
		map<int, GameObject*>& objects();

	private:
		map<int, GameObject*> m_gameObjects;
		
		GameCoordinate m_max;
		GameCoordinate m_min;
		bool m_searched;

};

