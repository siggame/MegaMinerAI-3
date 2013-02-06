#include "SExprSource.h"

//For va_*
#include <cstdarg>

bool SExprSource::writeSExpr(int n, ...)
{
	//Get the ...'s
	va_list argp;
	va_start(argp, n);
	
	string toWrite = "(";
	for(int i=0; i < (n-1); ++i)
	{
		toWrite += "\"" + string(va_arg(argp, char*)) + "\"";
	}
	toWrite += "\"" + string(va_arg(argp, char*)) + "\")";
	va_end(argp);

	return write(toWrite);
}
