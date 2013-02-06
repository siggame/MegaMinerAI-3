//Ben Murrell
#pragma once

#include "File.h"
#include "SExpr.h"
#include "SExprSource.h"

#include <deque>
using std::deque;

/* \brief This class represents a buffer that reads an entire File into memory, then acts
 *        as a source of s-expressions.
 */
class Buffer : public SExprSource
{
	public:
		Buffer();
		virtual ~Buffer();

		void readFile(File* file);

		//SExprSource functions
		virtual bool sexprReady(int msWait);
		virtual SExpr getSExpr();
		virtual bool endOfSource();
		virtual bool write(string toWrite);

	private:
		deque<SExpr> m_sexprs;
        File* m_file;
};
