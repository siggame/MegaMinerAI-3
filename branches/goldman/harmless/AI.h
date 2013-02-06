#ifndef AI_H
#define AI_H
#include <vector>
#include "time.h"
using namespace std;
#include "BaseAI.h"
/*Files you should probably look at
  BaseAi for data structures
  wrappers.h for functions
*/
struct location
{
  location(){};
  location(int X,int Y){x=X;y=Y;};
  void operator=(location rhs){x=rhs.x;y=rhs.y;};
  location operator +(location& rhs){return location(x+rhs.x,y+rhs.y);};
  void operator+=(location& rhs){x=x+rhs.x,y=y+rhs.y;};
  int x,y;
};

template <class T>
  //Pre:  T has the function x() and y() defined
  //Post: Returns the index of the T at x,y or if nothing is there, -1
int atLocation(vector<T> vect, int x,int y)
{
  for(unsigned int i=0;i<vect.size();i++)
  {
    if(vect[i].x() ==x && vect[i].y() == y)
      return int(i);
  }
  return -1;
}

template <class T>
int atLocation(vector<T> vect, location loc)
{
  return atLocation(vect,loc.x,loc.y);
}

#include <iostream>

class AI: public BaseAI
{
public:
  AI();

  
  void init(){};  
  
  vector<location> validMoves(int x, int y);
  
  int distance(int startx,int starty, int endx,int endy);
  
  vector<location> withinRange(int x, int y,int range);
  
  bool inBounds(int x, int y);
  
  virtual const char* username();
  virtual const char* password();
  virtual bool runHuman();
  virtual bool runZombie();
};

#endif
