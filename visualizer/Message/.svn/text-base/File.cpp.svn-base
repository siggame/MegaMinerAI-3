//Ben Murrell
#include "File.h"

#include <iostream>
using std::cout;
using std::endl;
using std::getline;

File::File()
{
	m_state = FileState::None;
}

 File::~File()
{
	close();
}

//Open/close
bool File::open(string path)
{
	//Already have a file open?
	if(m_state == FileState::Open || m_state == FileState::EOF)
	{
		cout << "File object cannot open a file when it already has a file open!" << endl;
		return false;
	}

	//Open the file
	m_file.open(path.c_str());

	//Check for failure
	if(m_file.fail())
	{
		cout << "Failed to open " << path << endl;
		m_state = FileState::Failed;
		return false;
	}

	m_state = FileState::Open;
	return true;
}

//Close the file
void File::close()
{
	if(m_file.is_open())
	{
		m_file.close();
	}

	m_state = FileState::Closed;
}

//Info
FileState::State File::state()
{
	return m_state;
}

//SExprSource functions
bool File::sexprReady(int msWait)
{
	return !endOfSource();
}

//Assumptions: SExpr's don't have any newline characters in them. SExpr's are on separate lines.
SExpr File::getSExpr()
{
	string toProcess;
	getline(m_file, toProcess);
	
	//Catch empty lines
	if(toProcess.size() == 0)
	{
		toProcess = "(null)";
	}

	SExpr ret(toProcess);

	return ret;
}

//Return true in all cases that we can no longer read from the file
bool File::endOfSource()
{
	bool ret = false;

	if(m_state != FileState::Open)
	{
		ret = true;
	}

	if(m_file.eof())
	{
		m_state = FileState::EOF;
		ret = true;
	}

	return ret;
}

bool File::write(string toWrite)
{
	return false;
}
