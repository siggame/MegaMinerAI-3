//Copyright 2009(C) Ben Murrell
//Missouri S&T ACM AI
#pragma once

#include "GameCoordinate.h"
#include "Animation.h"
#include "SExpr.h"


class GameObject
{
	public:
		GameObject();
		virtual ~GameObject();
        virtual GameObject& operator=(const GameObject& right);

		virtual string type() = 0;
		virtual void draw2d(float percentThere) = 0; //Draw the object in 2d at (0,0) facing towards (1,0)
		virtual void fromSExpr(const SExpr& from);
		virtual GameObject* clone() = 0;

		int id();
		GameCoordinate& location();
		Animation& animation();

		//Stats
		int curHealth();
		int maxHealth();
		int overHealth();

		void setID(int id);
		void setCurHealth(int curHealth);
		void setMaxHealth(int maxHealth);
		void setOverHealth(int overHealth);
	
	protected:
		int m_id;
		GameCoordinate m_location;
		int m_curHealth;
		int m_maxHealth;
		int m_overHealth;
        Animation m_animation;
};
