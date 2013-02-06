//Ben Murrell
#pragma once


#include "sexp.h"
#include "SExprManager.h"
#include <string>
using std::string;


namespace NodeType
{
	enum SExprNodeType { Value, List, Unknown };
};

/* \brief This class wraps the sexp library to have a more object oriented API.
 *        It relies on the SExprManager class to manage its memory via
 *        reference counting.
 */
class SExpr
{
	public:
		//Maintenance
		SExpr();
		SExpr(string init);
		~SExpr();
		SExpr(const SExpr& copy);
		SExpr& operator=(const SExpr& right);

		//IO
		void fromString(string from);
		string toString();

		//Access
		NodeType::SExprNodeType type();
		SExpr next();
		SExpr list();

		int numElements();
		SExpr element(int n);
		string operator[](int n) const;
		string value();

	private:
		sexp_t* m_root;
		sexp_t* m_sexp;
		int m_numElements;
        SExprManager* m_manager;

		sexp_t* operator()(int n) const;

};
