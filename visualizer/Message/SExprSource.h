//Ben Murrell
#pragma once

#include "SExpr.h"

#include <string>
using std::string;

/* \brief This class is a pure abstract class that creates an interface for generic SExpr
 *        sources.
 */
class SExprSource
{
	public:
		SExprSource() {};
		virtual ~SExprSource() {};

		virtual bool sexprReady(int msWait=0) = 0;
		virtual SExpr getSExpr() = 0;
		virtual bool endOfSource() = 0;

		virtual bool write(string toWrite) = 0;
		virtual bool writeSExpr(int n, ...);
};
