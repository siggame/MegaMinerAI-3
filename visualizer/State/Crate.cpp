
#include "Crate.h"
#include "Engine.h"

Crate::Crate()
{

}

Crate::~Crate()
{

}

Crate& Crate::operator=(const Crate& right)
{
    GameObject::operator=(right);
    return *this;
}

GameObject* Crate::clone()
{
	Crate* tmp = new Crate();
    *tmp = *this;
	return tmp;
}

string Crate::type()
{
	return "Crate";
}

void Crate::draw2d(float percentThere)
{
	GLfloat center[] = {0.0f, 0.0f};
	glColor3ub(139,69,19);
	Engine::instance()->drawCircle(center, 1.0f);    
}
