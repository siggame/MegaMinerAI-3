#pragma once

#include "GameObject.h"

#include <SDL.h>
#include <SDL_opengl.h>
#include <GL/gl.h>

class Human : public GameObject
{
	public:
		Human();
		virtual ~Human();
        virtual Human& operator=(const Human& right);

		virtual string type();
        virtual void fromSExpr(const SExpr& from);
		virtual GameObject* clone();
        virtual void draw2d(float percentThere);

		void drawCircleOutline(GLfloat* center, GLfloat radius);
		void createOutlineList();
		void filledCircle(GLfloat* center, GLfloat radius, GLfloat percentFull);

	private:
        unsigned int m_sight;
        unsigned int m_weapon;

};
