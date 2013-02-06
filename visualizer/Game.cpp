//Ben Murrell
#include "Game.h"
#include "Config.h"
#include "Engine.h"


Game::Game()
{
	m_sexprSource = 0;
	m_prevTurn = 0;
    m_stateLock = SDL_CreateMutex();
}

Game::~Game()
{
	reset();
}

GameCoordinate Game::minCoord()
{
	return m_minCoord;
}

GameCoordinate Game::maxCoord()
{
	return m_maxCoord;
}

int Game::size()
{
	return m_states.size();	
}

GameState* Game::operator[](int index)
{
	//This needs to be threadsafe
    SDL_LockMutex(m_stateLock);

	m_prevTurn = index;
    GameState* ret = m_states[index];

    SDL_UnlockMutex(m_stateLock);

	return ret;
}

void Game::updateWalls(GameState* state)
{
    //Add/update curwalls, copy curwalls into the state being pushed
	map<int, GameObject*>& objects = state->map().objects();
	for(map<int, GameObject*>::iterator cursor = objects.begin(); cursor != objects.end(); cursor++)
	{
        //Is the gameobject a wall?
        Wall* wall = dynamic_cast<Wall*>(cursor->second);
        if(wall != 0)
        {
            //Did we know this wall existed?
            if(m_curWalls.count(wall->id()) == 0)
            {
                //No - make a copy of it
                m_curWalls[wall->id()] = static_cast<Wall*>(wall->clone());
            }
            else
            {
                //Yes - update our copy of it
                *(m_curWalls[wall->id()]) = *wall;
            }
        }
    }

    //Now copy m_curWalls into the list of objects for the state
	for(map<int, Wall*>::iterator cursor = m_curWalls.begin(); cursor != m_curWalls.end(); cursor++)
    {
        //Does the state being pushed already have the wall in question?
        if(objects.count(cursor->first) == 0)
        {
            //No - Make a copy and put it in there
            cursor->second->setWithinView(false);
            objects[cursor->first] = cursor->second->clone();
        }
    }
}


#include <iostream>
using std::endl;
using std::cout;
//I guess this could technically be a member of GameState... I really need to document all of this stuff
//Goal: 
//      remove objects that haven't appeared yet
void Game::stripState(GameState* state)
{
    deque<Animation*>& animations = state->animations();
    map<int, GameObject*>& objects = state->map().objects();
    
    //For each animation
    for(unsigned int i=0; i < animations.size(); ++i)
    {
        //Hasn't appeared yet - remove from the state!
        if(animations[i]->type() == Animation::Add)
        {
            //Make sure it's something that isn't in the deadlist
            if(objects.count(animations[i]->ids()[0]) > 0)
            {
                //cout << "Removing " << objects[animations[i]->ids()[0]]->type() << " because it was in the state but has an appear animation in the state still" << endl;
                delete objects[animations[i]->ids()[0]];
                objects.erase(animations[i]->ids()[0]);
            }
        }
    }
}


void Game::pushState(GameState* state)
{   
	deque<Animation*>& animations = state->animations();
	GameState* copy;

	copy = new GameState();
	*copy = *state;
	animations = copy->animations();

	if(animations.size() == 0 || !Config::instance()->animate())
	{
        SDL_LockMutex(m_stateLock);
		m_states.push_back(state);
		updateBoundary(state);
		stripState(state);
        SDL_UnlockMutex(m_stateLock);
		
        delete copy;
		return;
	}

	while(animations.size() > 0)
	{
		splitState(copy);

		updateBoundary(copy);

        SDL_LockMutex(m_stateLock);
		m_states.push_back(copy);

		copy = new GameState();
		*copy = *m_states.back();
		animations = copy->animations();
		
		stripState(m_states.back());
        SDL_UnlockMutex(m_stateLock);

	}

	delete copy;
}


void Game::splitState(GameState* state)
{
    //Map object id's to animation types so we can tell if something has already been animated
    map<int, Animation::Type> animated;

    map<int, GameObject*>& objects = state->map().objects();
    deque<Animation*>& animations = state->animations();
    deque<GameObject*>& dead = state->dead();

    //Init our animated map & clear animations off of the gameobjects
    for(map<int, GameObject*>::iterator cursor = objects.begin(); cursor != objects.end(); cursor++)
    {
        Animation& animation = cursor->second->animation();

        animated[cursor->first] = Animation::None;
        animation.setType(Animation::None);
        animation.ids().clear();
    }

    //For each animation...
	for(deque<Animation*>::iterator cursor = animations.begin(); cursor != animations.end();)
    //for(unsigned int i=0; i < animations.size(); ++i)
    {
        deque<int>& ids = (*cursor)->ids();

        bool ok=true;
        for(unsigned int j=0; j < ids.size(); ++j)
        {
            //Check that the object isn't already being animated in this substate - but if it's just getting hurt alot/removed, that's ok
			if(animated[ids[j]] != Animation::None)
			{
				ok = false;
			}

			if(((*cursor)->type() == Animation::Hurt || ((*cursor)->type() == Animation::Remove) && (animated[ids[j]] == Animation::Hurt)))
			{
				ok = true;
			}
        }

        //If we can't continue in this substate, start a new one
        if(!ok)
        {
			cursor++;
            continue;
        }

        //For each ID affected by the animation
        for(unsigned int j=0; j < ids.size(); ++j)
        {
            animated[ids[j]] = (*cursor)->type();

            //Make sure that the object exists - if not, pull it from the deadlist
            bool existed = true;
            if(objects.count(ids[j]) == 0)
            {
                existed = false;
                for(unsigned int k=0; k < dead.size(); ++k)
                {
                    if(dead[k]->id() == ids[j])
                    {
                        objects[ids[j]] = dead[k]->clone();
                        existed = true;
                    }
                }
            }

            //Assign the animation (assuming the object existed)
            if(existed)
            {
				objects[ids[j]]->animation() = *(*cursor);
                
                if((*cursor)->type() == Animation::Move)
                {
                    objects[ids[j]]->location().setX((*cursor)->x());
                    objects[ids[j]]->location().setY((*cursor)->y());
                }
            }
        }
        
		//Ugh.
        //Strip the animation out of the state
        delete *cursor;
        animations.erase(cursor);
		cursor = animations.begin();
		if(cursor == animations.end())
		{
			break;
		}
    }
}


void Game::updateBoundary(GameState* state)
{
    //Update boundary coords
	if(state->map().max().x() > m_maxCoord.x())
		m_maxCoord.setX(state->map().max().x());
	
	if(state->map().max().y() > m_maxCoord.y())
		m_maxCoord.setY(state->map().max().y());

	if(state->map().min().x() < m_minCoord.x())
		m_minCoord.setX(state->map().min().x());
	
	if(state->map().min().y() < m_minCoord.y())
		m_minCoord.setY(state->map().min().y());

}

void Game::setSource(SExprSource* source)
{
	//Already have a source
	if(m_sexprSource != 0)
	{
		return;
	}

	m_sexprSource = source;
}

bool Game::pollSource()
{
	if(m_sexprSource == 0)
	{
		return false;
	}

	if(m_sexprSource->sexprReady())
	{
		SExpr sexpr = m_sexprSource->getSExpr();
		
		if(sexpr.type() == NodeType::Unknown)
		{
			return false;
		}
		
		if(sexpr[0] == "status")
		{
			GameState* state = new GameState;
			state->fromSExpr(sexpr);

			pushState(state);
			return true;
		}

        //Ident message
        //("ident" (("0" "MuesAI" "Stephen Mues" "human") ("1" "MuesAI" "Stephen Mues" "zombie")) "0")
		if(sexpr[0] == "ident")
		{
			SExpr ident = sexpr.next().list();
            if(ident.list()[3] == "zombie")
            {
			    m_zombieName = ident.list()[2];
			    m_humanName = ident.next().list()[2];
            }
            else
            {
			    m_humanName = ident.list()[2];
			    m_zombieName = ident.next().list()[2];
            }
		}

		if(sexpr[0] == "notification")
		{
            if(sexpr[1] == Config::instance()->get("user") || atoi(Config::instance()->get("arenaMode").c_str()) > 0)
			{
				cout << "A game was joined by the same player that this visualizer is logged in as" << endl;
                if( atoi(Config::instance()->get("autoJoin").c_str()) > 0 || atoi(Config::instance()->get("arenaMode").c_str()) > 0)
				{
                    cout << "Stopping reader thread" << endl;
					m_nextGame = sexpr[2];
                    Engine::instance()->stopPolling();
					//throw "die";
				}
			}
		}
	}

	return false;
}

void Game::reset()
{
	for(unsigned int i=0; i < m_states.size(); ++i)
	{
		delete m_states[i];
	}
	m_states.clear();

	for(map<int, Wall*>::iterator cursor = m_curWalls.begin(); cursor != m_curWalls.end(); cursor++)
	{
		delete cursor->second;
	}
	m_curWalls.clear();
}

string Game::zombieName()
{
	return m_zombieName;
}

string Game::humanName()
{
	return m_humanName;
}

string Game::nextGame()
{
	return m_nextGame;
}
