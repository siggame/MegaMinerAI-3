#include "Human.h"
#include "Engine.h"
#include "Config.h"

#include <cmath>

Human::Human()
{
    location().setHeading(1);
}

Human::~Human()
{

}

Human& Human::operator=(const Human& right)
{
    GameObject::operator=(right);
    m_sight = right.m_sight;
    m_weapon = right.m_weapon;
    return *this;
}

void Human::fromSExpr(const SExpr& from)
{
    GameObject::fromSExpr(from);

    m_weapon = atoi( from[8].c_str() );
    m_sight = atoi( from[9].c_str() );
}

GameObject* Human::clone()
{
	Human* tmp = new Human();
    *tmp = *this;
	return tmp;
}

string Human::type()
{
	return "Human";
}

#include <iostream>
using namespace std;
void Human::draw2d(float percentThere)
{
    static GLfloat center[] = { (float)atof(Config::instance()->get("humanOffsetX").c_str()), 
                                (float)atof(Config::instance()->get("humanOffsetY").c_str())};
    static GLfloat _radius = (float)atof(Config::instance()->get("humanSize").c_str());
	
    GLfloat radius = _radius;
    glColor3ub(255,0,0);

    //Attack animation
    if(m_animation.type() == Animation::Attack)
    {

        static GLfloat attackRadius = (float)atof(Config::instance()->get("humanAttackRadius").c_str());
        
        GLfloat humanPos[2];
        GLfloat actionPos[2];
        Engine::hexToRect((float)location().x(), (float)location().y(), humanPos);
        Engine::hexToRect((float)animation().x(), (float)animation().y(), actionPos);
        
        humanPos[0] = actionPos[0] - humanPos[0];
        humanPos[1] = actionPos[1] - humanPos[1];
		
		GLfloat distance = (float)sqrt(pow(humanPos[0],2) + pow(humanPos[1],2));
		GLfloat attack[2] = {center[0] + attackRadius + distance*percentThere, center[1]};

        GLfloat direction;
		if(humanPos[0] > 0)
		{
			direction = atan(humanPos[1]/humanPos[0])*180.0f/PI;
		}
		else if(humanPos[0] < 0)
		{
			direction = 180.0f + atan(humanPos[1]/humanPos[0])*180.0f/PI;	
		}
		else
		{
			if(humanPos[1] > 0)
				direction = 90.0f;
			else
				direction = 270.0f;
		}
		direction -= Engine::orientationToDegrees((float)location().heading());

        glPushMatrix();
        glRotatef(direction, 0, 0, 1);
        Engine::instance()->drawCircle(attack, attackRadius);

        glPopMatrix();

        /*
        if(percentThere > .3)
        {
		    glPushMatrix();
		    glRotatef(direction, 0, 0, 1);
            glTranslatef(distance, center[1], -0.5f);
		    glRotatef(30.0f, 0, 0, 1);
		    glColor3ub(255,127,0);
		    Engine::instance()->drawHexCircle(1);
		    glPopMatrix();
        }
        */
    }

    //Build animation - unfortunately, my modularity is breaking down here. Since the engine handles rotation
    //We have to counteract the rotation in here.
    if(m_animation.type() == Animation::Build)
    {
        glColor3ub(0,0,255);
        static GLfloat attackRadius = (float)atof(Config::instance()->get("humanAttackRadius").c_str());
        GLfloat attack[2] = {center[0] + attackRadius + 3.0f*percentThere, center[1]};
        
        GLfloat humanPos[2];
        GLfloat actionPos[2];
        Engine::hexToRect((float)location().x(), (float)location().y(), humanPos);
        Engine::hexToRect((float)animation().x(), (float)animation().y(), actionPos);
        
        humanPos[0] = actionPos[0] - humanPos[0];
        humanPos[1] = actionPos[1] - humanPos[1];

        GLfloat direction;
		if(humanPos[0] > 0)
		{
			direction = atan(humanPos[1]/humanPos[0])*180.0f/PI;
		}
		else if(humanPos[0] < 0)
		{
			direction = 180.0f + atan(humanPos[1]/humanPos[0])*180.0f/PI;	
		}
		else
		{
			if(humanPos[1] > 0)
				direction = 90.0f;
			else
				direction = 270.0f;
		}
		direction -= Engine::orientationToDegrees((float)location().heading());

        glPushMatrix();
        glRotatef(direction, 0, 0, 1);
        Engine::instance()->drawCircle(attack, attackRadius);
        glPopMatrix();

        glColor3ub(255,0,0);
    }
	filledCircle(center, radius, (float)m_curHealth/(float)m_overHealth);
	
	if(Engine::instance()->showHumanSight())
	{
		glPushMatrix();
		glTranslatef(0.0f, 0.0f, -0.5f);
		glRotatef(30.0f, 0, 0, 1);
		glColor3ub(255,255,255);
		Engine::instance()->drawHexCircle(m_sight);
		glPopMatrix();
	}
}



void Human::drawCircleOutline(GLfloat* center, GLfloat radius)
{
    static bool created = false;
    static GLuint list;
    if(!created)
    {
        createOutlineList();
        list = Engine::instance()->recallList("humanOutline");
        created = true;
    }

    glPushMatrix();
	glTranslatef(center[0], center[1], 0); //Translate out to where the circle is supposed to be
    glScalef(radius, radius, 0.0f);
    glCallList(list);
    glPopMatrix();
}

void Human::createOutlineList()
{
    GLuint list = Engine::instance()->registerList("humanOutline");
	glNewList(list, GL_COMPILE);
	
	GLfloat x;
	GLfloat y;
	GLfloat theta;
	int n = 16;

		glBegin(GL_LINE_STRIP);
			for(int i=0; i <= n; ++i)
			{
				theta = (i/static_cast<float>(n))*2.0f*PI;
				x = cos(theta);
				y = sin(theta);

				glVertex2f(x,y);
			}
		glEnd();
	glEndList();

}

void Human::filledCircle(GLfloat* center, GLfloat radius, GLfloat percentFull)
{
	drawCircleOutline(center, radius);
	
	GLfloat x;
	GLfloat y;
	GLfloat theta;
	int n = 32;

    glPushMatrix();
	glTranslatef(center[0], center[1], 0); //Translate out to where the circle is supposed to be
    glScalef(radius, radius, 0.0f);
	glRotatef(-30.0f, 0, 0, 1); //account for engine rotating us
	
	GLfloat maxHeight = 2*percentFull - 1;
	
	glBegin(GL_TRIANGLE_FAN);
		glVertex2f(0.0f, maxHeight);
		for(int i=0; i <= n; ++i)
		{
			theta = (i/static_cast<float>(n))*2.0f*PI;
			x = cos(theta);
			y = min(sin(theta), maxHeight);

			glVertex2f(x,y);
		}
	glEnd();
	glPopMatrix();
}


