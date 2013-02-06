#ifndef AI_H
#define AI_H
#include <vector>
#include <queue>
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
  bool operator==(location& rhs){return (x==rhs.x && y==rhs.y);};
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


//--------------------------A* CRAP
struct Zlocation
{
  Zlocation(){};
  Zlocation(int X,int Y,int f){x=X;y=Y;face=f;};
  void operator=(Zlocation rhs){x=rhs.x;y=rhs.y;face=rhs.face;};
  Zlocation operator +(Zlocation& rhs){return Zlocation(x+rhs.x,y+rhs.y,face=rhs.face);};
  bool operator==(Zlocation& rhs){return (x==rhs.x && y==rhs.y && face==rhs.face);};
  int x,y,face;
};

//PREVMOVE:
//LEFT = 1
//RIGHT = -1
//FORWARD = 0
struct Zwrapper
{
  Zlocation node;
  vector<short> movehistory;
  short hX, gX, prevmove, parent;
  Zwrapper(){};
  Zwrapper(Zlocation blah){node = blah;};
  Zwrapper(Zlocation blah, short H, short G, short PREV, vector<short> movehist);
  friend bool operator<(const Zwrapper& rhs, const Zwrapper& lhs);
  friend bool operator==(const Zwrapper& rhs, const Zwrapper& lhs);
  friend bool operator>(const Zwrapper& rhs, const Zwrapper& lhs);
};

///---------------------------------------

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
  
  //Pre:  Takes in a board state.
  //Post:  Modifies the moves vector into a list of legal moves.
  bool findmoves(Zlocation start);
  vector< vector<Zwrapper> > cList;

  bool pushZ(Zwrapper toPush);
  //Performs the actual IDDFG search algorithm.  Recursively does
  //DFTS until depth = -1, or until a goal state is found.
  vector<short> aStarSearch(Zlocation current, Zlocation goal);
  
};


#endif
