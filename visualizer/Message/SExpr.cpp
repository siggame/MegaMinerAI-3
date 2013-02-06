//Ben Murrell

#include "SExpr.h"
#include "SExprManager.h"

//Constructor
SExpr::SExpr()
{
	m_root = 0;
	m_sexp = 0;
	m_numElements = -1;
    m_manager = SExprManager::instance();
}

//Constructor
SExpr::SExpr(string init)
{
    m_root = 0;
	m_sexp = 0;
	m_numElements = -1;
    m_manager = SExprManager::instance();
	fromString(init);
}

//Destructor
SExpr::~SExpr()
{
	m_manager->release(m_root);
	m_root = 0;
    m_sexp = 0;
    m_numElements = -1;
}

//Copy constructor
SExpr::SExpr(const SExpr& copy)
{
	*this = copy;
}

//Assignment Operator
SExpr& SExpr::operator=(const SExpr& right)
{
    m_manager = right.m_manager;
	m_manager->acquire(right.m_root);
	m_manager->release(m_root);
	m_root = right.m_root;
	m_sexp = right.m_sexp;
	m_numElements = -1;
	return *this;
}

#include <iostream>
using namespace std;

//Import an s-expression from a string
void SExpr::fromString(string from)
{
	m_manager->release(m_root);
	m_numElements = -1;
	m_sexp = parse_sexp((char*)from.c_str(), from.size());
	m_root = m_sexp;
	m_manager->acquire(m_root);

	//cout << type() << endl;
	//cout << numElements() << endl;

	if(type() == NodeType::List && numElements() == 1)
	{
		m_sexp = m_sexp->list;
	}
}

//Convert an s-expression to a string
string SExpr::toString()
{
	int n = numElements();
	if(n == 0)
	{
		return "()";
	}

	//Use the low level access - using next() and/or element() is expensive in a full traversal
	sexp_t* cur = m_sexp;
	
	string ret = "(";
	for(int i=0; i < (n-1); ++i)
	{
		//If it's a list, descend
		if(cur->ty == SEXP_LIST)
		{
			SExpr tmp;
			tmp.m_sexp = cur->list;
			ret += tmp.toString() + " ";
		}
		else
		{
			ret += "\"" + string(cur->val) + "\" ";
		}

		cur = cur->next;
	}
	if(cur->ty == SEXP_LIST)
	{
		SExpr tmp;
		tmp.m_sexp = cur->list;
		ret += tmp.toString() + ")";
	}
	else
	{
		ret += "\"" + string(cur->val) + "\")";
	}

	return ret;
}

//Go to the next element in the expression - probably shouldn't be used - use element() / [] instead
// ele -> ele -> ele -> ele -> null
SExpr SExpr::next()
{
	SExpr ret = *this;
	ret.m_sexp = m_sexp->next;

	return ret;
}

//Descend into the list pointed to by the element
// ele -> ele -> ele -> list -> ele -> null
//                        |
//                       ele -> ele -> null
SExpr SExpr::list()
{
	SExpr ret = *this;
	ret.m_sexp = m_sexp->list;

	return ret;
}

//Get the number of elements in the expression (lists count as 1)
int SExpr::numElements()
{
	//Lazy - once we count, assume the tree is read only
	if(m_numElements > 0)
	{
		return m_numElements;
	}

	int count = 0;

	sexp_t* cur = m_sexp;
	
	while(cur != 0)
	{
		cur = cur->next;
		count ++;
	}
	
	m_numElements = count;
	return count;
}

//Return the nth element of the s-expression
SExpr SExpr::element(int n)
{
	SExpr ret = *this;
	ret.m_sexp = m_sexp;
	
	for(int i=0; i < n; ++i)
	{
		ret.m_sexp = ret.m_sexp->next;
	}

	return ret;
}

//Return the nth value of the s-expression
string SExpr::operator[](int n) const
{
	return string((*this)(n)->val);
}

//Return the nth sexp_t* - private!
sexp_t* SExpr::operator()(int n) const
{
	sexp_t* cur = m_sexp;
	for(int i=0; i < n; ++i)
	{
		cur = cur->next;
	}
	return cur;
}

//Return the value of the element
string SExpr::value()
{
	if(type() == NodeType::List)
	{
		return "list";
	}
	return string(m_sexp->val);
}

//Return the type of the node
NodeType::SExprNodeType SExpr::type()
{
	if(m_sexp == 0)
	{
		return NodeType::Unknown;
	}

	switch(m_sexp->ty)
	{
		case SEXP_BINARY:
		case SEXP_VALUE:
			return NodeType::Value;
			break;

		case SEXP_LIST:
			return NodeType::List;
			break;
		default:
			return NodeType::Unknown;
			break;
	}
}
