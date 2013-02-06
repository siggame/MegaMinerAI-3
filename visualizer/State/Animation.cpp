#include "Animation.h"

#include "SExpr.h"

Animation::Animation()
{
	m_x = 0;
	m_y = 0;
	m_type = None;
}

Animation::~Animation()
{

}

Animation& Animation::operator=(const Animation& right)
{
    m_x = right.m_x;
    m_y = right.m_y;
    m_type = right.m_type;
    m_ids = right.m_ids;

    return *this;
}

void Animation::fromSExpr(SExpr in)
{ 
	 m_ids.clear();

	//enum Type{None, Add, Remove, Move, Turn, Attack, Hurt, Eat, Grab, Throw, Give, Build};
	//Sigh
	if(in[0] == "add")
	{
      m_type = Add;
      m_ids.push_back(atoi( in[1].c_str() ));
	}
	else if(in[0] == "move")
	{
      m_type = Move;
      m_ids.push_back(atoi( in[1].c_str() ));
      m_x = atoi( in[2].c_str() );
      m_y = atoi( in[3].c_str() );
	}
	else if(in[0] == "remove")
	{
      m_type = Remove;
      m_ids.push_back(atoi( in[1].c_str() ));
	}
	else if(in[0] == "attack")
	{
		m_type = Attack;
		m_ids.push_back(atoi( in[1].c_str() ));
		m_x = atoi( in[2].c_str() );
		m_y = atoi( in[3].c_str() );
	}
	else if(in[0] == "hurt")
	{
		m_type = Hurt;

		SExpr ids = in.next().list();
	  
		int count = ids.numElements();
		for(int i=0; i < count; ++i)
		{
			m_ids.push_back( atoi( ids[0].c_str() ));
			ids = ids.next();
		}
   }
   else if(in[0] == "turn")
   {
		m_type = Turn;
		m_ids.push_back(atoi( in[1].c_str() ));
		m_x = atoi( in[2].c_str() ); //Facing direction
   }
   //enum Type{None, Add, Remove, Move, Turn, Attack, Hurt, Eat, Grab, Throw, Give, Build};
   else if(in[0] == "eat")
   {
		m_type = Eat;
		m_ids.push_back(atoi( in[1].c_str() ));
		m_ids.push_back(atoi( in[2].c_str() ));
   }
   else if(in[0] == "grab")
   {
		m_type = Grab;
		m_ids.push_back(atoi( in[1].c_str() ));
		m_ids.push_back(atoi( in[2].c_str() ));
   }
   else if(in[0] == "throw")
   {
		m_type = Throw;
		m_ids.push_back(atoi( in[1].c_str() ));
		m_ids.push_back(atoi( in[2].c_str() ));
		m_x = atoi( in[3].c_str() );
		m_y = atoi( in[4].c_str() );
	}
	else if(in[0] == "give")
	{
		m_type = Give;
		m_ids.push_back(atoi( in[1].c_str() ));
		m_ids.push_back(atoi( in[2].c_str() ));
	}
	else if(in[0] == "build")
	{
		m_type = Build;
		m_ids.push_back(atoi( in[1].c_str() ));
		m_x = atoi( in[2].c_str() );
		m_y = atoi( in[3].c_str() );
	}
	else
	{
		//We don't know what this was...
		m_type = None;
	}   
}

Animation::Type Animation::type()
{
   return m_type;
}

void Animation::setType(Animation::Type type)
{
   m_type = type;
}


int Animation::x()
{
   return m_x;
}

int Animation::y()
{
   return m_y;
}

deque<int>& Animation::ids()
{
   return m_ids;
}

void Animation::setX(int x)
{
   m_x = x;
}

void Animation::setY(int y)
{
   m_y = y;
}
