#include "Zombie.h"
#include "Engine.h"
#include "Config.h"

Zombie::Zombie()
{

}

Zombie::~Zombie()
{

}

Zombie& Zombie::operator=(const Zombie& right)
{
    GameObject::operator=(right);
    m_location.setHeading(right.m_location.heading());
	m_sight = right.m_sight;
    
    return *this;
}

GameObject* Zombie::clone()
{
	Zombie* tmp = new Zombie();
    *tmp = *this;
	return tmp;
}

string Zombie::type()
{
	return "Zombie";
}


void Zombie::fromSExpr(const SExpr& from)
{
	GameObject::fromSExpr(from);

    m_sight = atoi( from[9].c_str() );
	location().setHeading( atoi(from[12].c_str()) );
}

void Zombie::draw2d(float percentThere)
{
    static GLfloat center[] = { (float)atof(Config::instance()->get("zombieOffsetX").c_str()), 
                                (float)atof(Config::instance()->get("zombieOffsetY").c_str())};
    static GLfloat _radius = (float)atof( Config::instance()->get("zombieSize").c_str() );

    GLfloat radius = _radius;
	glColor3ub(0,255,0);

    //Appear animation
    if(m_animation.type() == Animation::Add)
    {
        radius = percentThere * 0.8f;
    }

    //Attack animation
    if(m_animation.type() == Animation::Attack)
    {
        static GLfloat attackRadius = (float)atof(Config::instance()->get("zombieAttackRadius").c_str());
        GLfloat attack[2] = {center[0] + attackRadius + 3.0f*percentThere, center[1]};
        Engine::instance()->drawCircle(attack, attackRadius);
    }

    drawChevron(center, radius);

	if(Engine::instance()->showZombieSight())
	{
		glPushMatrix();
		glTranslatef(0.0f, 0.0f, -0.5f);
		glRotatef(30.0f, 0, 0, 1);
		glColor3ub(255,255,255);
		Engine::instance()->drawHexCircle(m_sight);
		glPopMatrix();
	}
}


void Zombie::drawChevron(GLfloat* center, GLfloat radius)
{
    static bool created = false;
    static GLuint list;
    if(!created)
    {
        createChevronList();
        list = Engine::instance()->recallList("zombieChevron");
        created = true;
    }

    glPushMatrix();
	glTranslatef(center[0], center[1], 0); //Translate out to where the circle is supposed to be
    glScalef(radius, radius, 0.0f);
    glCallList(list);
    glPopMatrix();
}

void Zombie::createChevronList()
{
    GLuint list = Engine::instance()->registerList("zombieChevron");
	glNewList(list, GL_COMPILE);

		glBegin(GL_TRIANGLES);
			glVertex2f(0,-0.7f);
			glVertex2f(0.7f, 0.0f);
            glVertex2f(0, 0.7f);
		glEnd();
	glEndList();

}
