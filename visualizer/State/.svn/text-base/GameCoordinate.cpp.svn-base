#include "GameCoordinate.h"

#include <math.h>
#include <algorithm>
using std::max;

GameCoordinate::GameCoordinate()
{
	m_gameX = 0;
	m_gameY = 0;
	m_gameHeading = 0;
}

GameCoordinate::~GameCoordinate()
{

}

GameCoordinate::GameCoordinate(const GameCoordinate& copy)
{
	*this = copy;
}

GameCoordinate& GameCoordinate::operator=(const GameCoordinate& copy)
{
	m_gameX = copy.m_gameX;
	m_gameY = copy.m_gameY;
	m_gameHeading = copy.m_gameHeading;

	return *this;
}

int GameCoordinate::distanceTo(const GameCoordinate& to) const
{
	int dx = m_gameX - to.x();
	int dy = m_gameY - to.y();
	int distance;

	if(dx*dy > 0)
		distance = abs(dx) + abs(dy);
	else
		distance = max( abs(dx), abs(dy) );
				
	return distance;
}


int GameCoordinate::x() const
{
	return m_gameX;
}

int GameCoordinate::y() const
{
	return m_gameY;
}

int GameCoordinate::heading() const
{
	return m_gameHeading;
}

void GameCoordinate::set(int x, int y, int heading)
{
	m_gameX = x;
	m_gameY = y;
	m_gameHeading = heading;
}

void GameCoordinate::setX(int x)
{
	m_gameX = x;
}

void GameCoordinate::setY(int y)
{
	m_gameY = y;
}

void GameCoordinate::setHeading(int heading)
{
	m_gameHeading = heading;
}
