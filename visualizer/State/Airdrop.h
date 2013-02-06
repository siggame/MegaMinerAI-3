#pragma once

#include "GameObject.h"

class Airdrop : public GameObject
{
	public:
		Airdrop();
		virtual ~Airdrop();
        virtual Airdrop& operator=(const Airdrop& right);

		virtual string type();
		virtual GameObject* clone();
		virtual void fromSExpr(const SExpr& from);
        virtual void draw2d(float percentThere);
	
	private:
		int m_remaining;
		int m_contents;

};
