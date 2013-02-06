
#include "GameState.h"

#include "GameObjectFactory.h"
#include "Animation.h"

#include <map>
using std::map;

GameState::GameState()
{
	m_numZombies = -1;
	m_humanHealth = -1;
}

GameState::~GameState()
{
   for(unsigned int i=0; i < m_animations.size(); ++i)
   {
      delete m_animations[i];
   }

   for(unsigned int i=0; i < m_dead.size(); ++i)
   {
       delete m_dead[i];
   }
}

GameState& GameState::operator=(const GameState& right)
{

    m_map = right.m_map;

    m_curTurn = right.m_curTurn;
    m_humansReady = right.m_humansReady;
    m_airstrikesReady = right.m_airstrikesReady;
    m_zombiesReady = right.m_zombiesReady;
    m_zombieCap = right.m_zombieCap;

    //Deep copy
    m_dead = right.m_dead;
    for(unsigned int i=0; i < m_dead.size(); ++i)
    {
        m_dead[i] = m_dead[i]->clone();
    }

    //Deep copy
    m_animations = right.m_animations;
    for(unsigned int i=0; i < m_animations.size(); ++i)
    {
        Animation* animation = new Animation;
        *animation = *m_animations[i];
        m_animations[i] = animation;
    }

    return *this;
}


//Convert an s-expression into the game state
void GameState::fromSExpr(SExpr from)
{
	//We've got problems!
	if(from[0] != "status")
	{
		return;
	}

	GameObjectFactory* factory = GameObjectFactory::instance();
	SExpr category = from;
	SExpr categoryInfo;

	for(int i=1; i < from.numElements(); ++i)
	{
		category = category.next();
		categoryInfo = category.list();
		
		//Possibility: Make Game a gameobject
		if(categoryInfo[0] == "Game")
		{
			setCurTurn( atoi(categoryInfo[1].c_str()) );
			setHumansReady( atoi(categoryInfo[2].c_str()) );
			setAirstrikesReady( (float)atof(categoryInfo[3].c_str()) );
			setZombiesReady( atoi(categoryInfo[4].c_str()) );
			setZombieCap( atoi(categoryInfo[5].c_str()) );
		}

		//Animations aren't game objects (I don't like how this is being done, but I don't think the right solution is to make a GameObject type for every type of animation... or to inherit from GameObject and have to cast all of the GameObjects into Animation*'s to find the animations.)
		//Other options: Have animations not be part of the status message... but this makes attaching the animations to the correct states pretty difficult
		else if(categoryInfo[0] == "Animations")
		{
			SExpr object = categoryInfo.next();
			SExpr objectInfo = object.list();
				
			object = categoryInfo;
			for(int i=1; i < categoryInfo.numElements(); ++i)
			{
				object = object.next();
				objectInfo = object.list();
					
				Animation* animation = new Animation();
				animation->fromSExpr(objectInfo);

				m_animations.push_back( animation );
			}
		}

		//Deadlist - Objects that were created & destroyed in the same turn
		//Normally wouldn't show up in state, but we want them to show up in the vis
		else if(categoryInfo[0] == "Dead")
		{
			SExpr object = categoryInfo.next();
			SExpr objectInfo = object.list();
				
			object = categoryInfo;
			for(int i=1; i < categoryInfo.numElements(); ++i)
			{
				object = object.next();
				objectInfo = object.list();
					
				GameObject* dead = factory->create(objectInfo);
				if(dead != 0)
				{
					m_dead.push_back(dead);
				}
			}

		}

		//Make sure we're going to dive into a list
		else if(categoryInfo.next().type() == NodeType::List)
		{
			//See if what's in this list is handled by the factory
			//Assumption: All lists contain like objects
			SExpr object = categoryInfo.next();
			SExpr objectInfo = object.list();
			
			if(!factory->handles(objectInfo[0]))
			{
				//cout << "Factory doesn't handle " << objectInfo[0] << endl;
				continue;
			}
			
			//The factory handles whatever is in this list - put the factory to work
			object = categoryInfo;
			for(int i=1; i < categoryInfo.numElements(); ++i)
			{
				object = object.next();
				objectInfo = object.list();
				
				m_map.addObject( factory->create(objectInfo) );
			}
		}
	}

    //Done importing the state from the SExpr
    //associateAnimations();
}




Map& GameState::map()
{
   return m_map;
}

deque<Animation*>& GameState::animations()
{
   return m_animations;
}

deque<GameObject*>& GameState::dead()
{
    return m_dead;
}

int GameState::curTurn()
{
	return m_curTurn;
}

int GameState::humansReady()
{
	return m_humansReady;
}

float GameState::airstrikesReady()
{
	return m_airstrikesReady;
}

int GameState::zombiesReady()
{
	return m_zombiesReady;
}

int GameState::zombieCap()
{
	return m_zombieCap;
}

void GameState::setCurTurn(int number)
{
	m_curTurn = number;
}

void GameState::setHumansReady(int humansReady)
{
	m_humansReady = humansReady;
}

void GameState::setAirstrikesReady(float airstrikesReady)
{
	m_airstrikesReady = airstrikesReady;
}

void GameState::setZombiesReady(int zombiesReady)
{
	m_zombiesReady = zombiesReady;
}

void GameState::setZombieCap(int zombieCap)
{
	m_zombieCap = zombieCap;
}

int GameState::numZombies()
{
	if(m_numZombies == -1)
	{
		m_numZombies = 0;
		for(std::map<int, GameObject*>::iterator cursor = m_map.objects().begin(); cursor != m_map.objects().end(); cursor++)
		{
			Zombie* tmp = dynamic_cast<Zombie*>(cursor->second);
			if(tmp)
			{
				m_numZombies++;
			}
		}
	}
	return m_numZombies;
}

int GameState::humanHealth()
{
	if(m_humanHealth == -1)
	{
		m_humanHealth = 0;
		for(std::map<int, GameObject*>::iterator cursor = m_map.objects().begin(); cursor != m_map.objects().end(); cursor++)
		{
			Human* tmp = dynamic_cast<Human*>(cursor->second);
			if(tmp)
			{
				m_humanHealth += tmp->curHealth();
			}
		}
	}
	return m_humanHealth;
}
