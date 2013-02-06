#pragma once

#include "GameObject.h"

#include <SDL.h>
#include <SDL_opengl.h>
#include <GL/gl.h>

class Wall : public GameObject
{
	public:
		Wall();
		virtual ~Wall();
        Wall& operator=(const Wall& right);

		virtual string type();
		virtual GameObject* clone();
        virtual void draw2d(float percentThere);

        void setWithinView(bool visible);
        bool withinView();

        void drawCircleOutline(GLfloat* center, GLfloat radius);
        void createOutlineList();
        void filledCircle(GLfloat* center, GLfloat radius, GLfloat percentFull);

	private:
        bool m_withinView;
};
