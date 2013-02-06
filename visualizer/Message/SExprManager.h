//Ben Murrell
#pragma once

#include <map>
using std::map;

#include "sexp.h"

/* \brief This class manages reference counting for SExpr. SExpr registers the base
 *        sexp_t*'s via acquire, then reference counts until the count is 0. When
 *        the count is 0, the base sexp_t* is destroyed.
 */
class SExprManager
{
	public:
		static SExprManager* instance();
		~SExprManager();

		void acquire(sexp_t* acquired);
		void release(sexp_t* released);

	private:
		SExprManager();
		map<sexp_t*, int> m_refCount;
};
