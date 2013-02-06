//Ben Murrell
#pragma once

#include "SExpr.h"
#include "SExprSource.h"
#include "sexp.h"

#include <string>
#include <fstream>
using std::string;
using std::ifstream;

//EOF is a macro - we want to use it
#undef EOF
namespace FileState
{
	enum State{ None, Open, EOF, Closed, Failed };
};


/* \brief This class represents a read only file that contains s-expressions.
 */
class File : public SExprSource
{
	public:
		File();
		virtual ~File();

		//Open/close
		bool open(string path);
		void close();

		//Info
		FileState::State state();

		//SExprSource functions
		virtual bool sexprReady(int msWait);
		virtual SExpr getSExpr();
		virtual bool endOfSource();
		virtual bool write(string toWrite);

	private:
		ifstream m_file;	
		FileState::State m_state;
};
