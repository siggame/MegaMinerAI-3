#pragma once

#include "GameObject.h"

#include <SDL.h>
#include <SDL_opengl.h>
#include <GL/gl.h>

class Zombie : public GameObject
{
	public:
		Zombie();
		virtual ~Zombie();
        virtual Zombie& operator=(const Zombie& right);

		virtual string type();
		virtual GameObject* clone();
		virtual void fromSExpr(const SExpr& from);
        virtual void draw2d(float percentThere);

        void drawChevron(GLfloat* center, GLfloat radius);
        void createChevronList();
	private:
        unsigned int m_sight;


};
