//Ben Murrell
#include "SExprManager.h"

#include <iostream>
using std::cout;
using std::endl;

SExprManager::SExprManager()
{
}

//Singleton
SExprManager* SExprManager::instance()
{
	static SExprManager* inst = new SExprManager();
	return inst;
}

SExprManager::~SExprManager()
{

}

//Acquire a root sexpr_t
void SExprManager::acquire(sexp_t* acquired)
{
	if(m_refCount.count(acquired) > 0 && acquired != 0)
	{
		m_refCount[acquired]++;
		//cout << "Incremented " << acquired << " to: " << m_refCount[acquired] << endl;
	}
	else if(acquired != 0)
	{
		m_refCount[acquired] = 1;
		//cout << "Acquired " << acquired << " for the first time" << endl;
	}
}

//Release a root sexpr_t
void SExprManager::release(sexp_t* released)
{
	if(m_refCount.count(released) > 0 && released != 0)
	{
		m_refCount[released]--;
		//cout << "Decremented " << released << " to: " << m_refCount[released] << endl;
		if(m_refCount[released] <= 0)
		{
			m_refCount.erase(released);
			destroy_sexp(released);
			//cout << "Freed sexp" << endl;
		}
	}
}
