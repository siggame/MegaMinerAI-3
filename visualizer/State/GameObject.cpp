//Copyright 2009(C) Ben Murrell
//Missouri S&T ACM AI
#include "GameObject.h"



GameObject::GameObject()
{

}

GameObject::~GameObject()
{

}

GameObject& GameObject::operator=(const GameObject& right)
{
    this->m_id = right.m_id;
    this->m_location = right.m_location;
    this->m_animation = right.m_animation;
    this->m_curHealth = right.m_curHealth;
    this->m_overHealth = right.m_overHealth;

    return *this;
}

//Generic fromSExpr that captures the id, location, and health info - HittableObjects on the server
void GameObject::fromSExpr(const SExpr& from)
{
	setID( atoi(from[1].c_str()) );
	location().setX( atoi(from[2].c_str()) );
	location().setY( atoi(from[3].c_str()) );
	setCurHealth( atoi(from[4].c_str()) );
	setOverHealth( atoi(from[5].c_str()) );
}

GameCoordinate& GameObject::location()
{
	return m_location;
}

Animation& GameObject::animation()
{
    return m_animation;
}

int GameObject::id()
{
	return m_id;
}

void GameObject::setID(int id)
{
	m_id = id;
}

//Stats
int GameObject::curHealth()
{
	return m_curHealth;
}

int GameObject::maxHealth()
{
	return m_maxHealth;
}

int GameObject::overHealth()
{
	return m_overHealth;
}

void GameObject::setCurHealth(int curHealth)
{
	m_curHealth = curHealth;
}

void GameObject::setMaxHealth(int maxHealth)
{
	m_maxHealth = maxHealth;
}

void GameObject::setOverHealth(int overHealth)
{
	m_overHealth = overHealth;
}

