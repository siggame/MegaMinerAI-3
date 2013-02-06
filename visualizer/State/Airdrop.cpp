
#include "Airdrop.h"
#include "Engine.h"

Airdrop::Airdrop()
{

}

Airdrop::~Airdrop()
{

}

Airdrop& Airdrop::operator=(const Airdrop& right)
{
    GameObject::operator=(right);
    m_remaining = right.m_remaining;
    m_contents = right.m_contents;
    return *this;
}

GameObject* Airdrop::clone()
{
	Airdrop* tmp = new Airdrop();
    *tmp = *this;
	return tmp;
}

string Airdrop::type()
{
	return "Airstrike";
}

void Airdrop::draw2d(float percentThere)
{
	GLfloat center[] = {0.0f, 0.0f};
	glColor3ub(0,0,0);
	Engine::instance()->drawCircle(center, (1.0f/(1+m_remaining-percentThere)));
}

void Airdrop::fromSExpr(const SExpr& from)
{
	GameObject::fromSExpr(from);

	m_remaining = atoi(from[4].c_str());
	m_contents = atoi(from[5].c_str());
}