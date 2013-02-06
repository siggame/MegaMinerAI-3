#pragma once

#include "GameObject.h"

class Crate : public GameObject
{
	public:
		Crate();
		virtual ~Crate();
        virtual Crate& operator=(const Crate& right);

		virtual string type();
		virtual GameObject* clone();
        virtual void draw2d(float percentThere);

	private:


};
