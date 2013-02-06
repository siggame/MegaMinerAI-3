
#include "Wall.h"
#include "Engine.h"

#include <cmath>

Wall::Wall()
{
    m_withinView = true;
    location().setHeading(1);
}

Wall::~Wall()
{

}

Wall& Wall::operator=(const Wall& right)
{
    GameObject::operator=(right);
    this->m_withinView = right.m_withinView;

    return *this;
}

GameObject* Wall::clone()
{
	Wall* tmp = new Wall();
    *tmp = *this;

	return tmp;
}

string Wall::type()
{
	return "Wall";
}


using namespace std;
void Wall::draw2d(float percentThere)
{
	//Todo: half cell piece that is rotated by engine
	GLfloat center[] = {0.0f, 0.0f};
	
    if(m_withinView)
        glColor3ub(0,0,255);
    else
        glColor3ub(0, 150, 255);

    GLfloat radius = 1.0f;
    if(m_curHealth > 600.0f)
    {
        radius = min(2.0f, (float)m_curHealth / 600.0f);
    }
    
    GLfloat health = min((float)m_curHealth / 600.0f, 1.0f);

    filledCircle(center, radius, health);  
}

void Wall::setWithinView(bool visible)
{
    m_withinView = visible;
}

bool Wall::withinView()
{
    return m_withinView;
}

void Wall::drawCircleOutline(GLfloat* center, GLfloat radius)
{
    static bool created = false;
    static GLuint list;
    if(!created)
    {
        createOutlineList();
        list = Engine::instance()->recallList("zombieOutline");
        created = true;
    }

    glPushMatrix();
	glTranslatef(center[0], center[1], 0); //Translate out to where the circle is supposed to be
    glScalef(radius, radius, 0.0f);
    glCallList(list);
    glPopMatrix();
}

void Wall::createOutlineList()
{
    GLuint list = Engine::instance()->registerList("zombieOutline");
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

void Wall::filledCircle(GLfloat* center, GLfloat radius, GLfloat percentFull)
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
