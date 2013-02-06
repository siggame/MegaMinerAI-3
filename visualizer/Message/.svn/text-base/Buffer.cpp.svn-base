//Ben Murrell
#include "Buffer.h"


Buffer::Buffer()
{
}

Buffer::~Buffer()
{
}


//Read in the file
void Buffer::readFile(File* file)
{
    m_file = file;
	while(!file->endOfSource() && m_sexprs.size() < 50)
	{
		this->m_sexprs.push_back( file->getSExpr() );
	}
}


//SExprSource functions
bool Buffer::sexprReady(int msWait)
{
	return !endOfSource();
}

SExpr Buffer::getSExpr()
{
	//Return m_sexprs[m_curPos] *then* increment curPos
    if(m_sexprs.size() < 25 && !m_file->endOfSource())
    {
        while(!m_file->endOfSource() && m_sexprs.size() < 50)
	    {
		    this->m_sexprs.push_back( m_file->getSExpr() );
	    }
    }

	SExpr ret = m_sexprs.front();
	m_sexprs.pop_front();
	return ret;
}

bool Buffer::endOfSource()
{
	return (m_sexprs.size() == 0 && m_file->endOfSource());
}

bool Buffer::write(string toWrite)
{
	return false;
}
