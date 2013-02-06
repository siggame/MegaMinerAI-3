
#include "Map.h"
#include "Engine.h"

Map::Map()
{
	m_searched = false;
}

Map::~Map()
{
	//Clean up
	for(map<int, GameObject*>::iterator cursor = m_gameObjects.begin(); cursor != m_gameObjects.end(); cursor++)
	{
		delete cursor->second;
	}
}

Map& Map::operator=(const Map& right)
{
    m_max = right.m_max;
	m_min = right.m_min;
    m_searched = right.m_searched;

    //Deep copy
    m_gameObjects = right.m_gameObjects;
	for(map<int, GameObject*>::iterator cursor = m_gameObjects.begin(); cursor != m_gameObjects.end(); cursor++)
	{
		cursor->second = cursor->second->clone();
	}

    return *this;
}

GameCoordinate& Map::max()
{
	if(m_searched)
	{
		return m_max;
	}
	else
	{
		min();
	}
	return m_max;
}

GameCoordinate& Map::min()
{
	if(m_searched)
	{
		return m_min;
	}

	for(map<int, GameObject*>::iterator cursor = m_gameObjects.begin(); cursor != m_gameObjects.end(); cursor++)
	{
		GameCoordinate& location = cursor->second->location();

		if(location.x() > m_max.x())
			m_max.setX(location.x());
		
		if(location.y() > m_max.y())
			m_max.setY(location.y());

		if(location.x() < m_min.x())
			m_min.setX(location.x());
		
		if(location.y() < m_min.y())
			m_min.setY(location.y());
	}

	return m_min;
}

void Map::addObject(GameObject* object)
{
	if(object == 0)
		return;
   
	m_gameObjects[object->id()] = object;
}


map<int, GameObject*>& Map::objects()
{
   return m_gameObjects;
}