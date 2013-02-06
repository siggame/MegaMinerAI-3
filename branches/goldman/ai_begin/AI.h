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
  location& operator=(const location& rhs)
  {
  x=rhs.x;
  y=rhs.y; 
  return *this;
  };
  location operator +(location& rhs){return location(x+rhs.x,y+rhs.y);};
  location operator -(location& rhs){return location(x-rhs.x,y-rhs.y);};
  void operator+=(location rhs){x=x+rhs.x,y=y+rhs.y;};
  bool operator==(location& rhs){return x==rhs.x && y==rhs.y;};
  location operator*(int rhs){ return location(x*rhs,y*rhs);};
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
  //Pre:  T has the function x() and y() defined
  //Post: Returns the index of the T at x,y or if nothing is there, -1
int watLocation(vector<T> vect, int x,int y)
{
  for(unsigned int i=0;i<vect.size();i++)
  {
    if(vect[i].x() ==x && vect[i].y() == y && vect[i].content() != -1)
      return int(i);
  }
  return -1;
}


int distance(int startX, int startY, int targetX, int targetY)
{
  int dis;
  int dx = targetX - startX;
  int dy = targetY - startY;
  int absdy = (dy<0?-dy:dy);
  int absdx = (dx<0?-dx:dx);
  if (dx * dy > 0)
    dis = absdx+absdy;
  else
    dis = (absdx>absdy?absdx:absdy);
  return dis;
}


template <class T>
int atLocation(vector<T> vect, location loc)
{
  return atLocation(vect,loc.x,loc.y);
}

#include <iostream>
//Pre:  T has the function x() and y() defined
//Post: Returns the index of the T nearest x,y or if nothing is there, -1
template <class T>
int nearest(vector<T> vect, int x, int y)
{
  int index=-1,dis=9999,temp;
  for(unsigned int i=0;dis>0 && i<vect.size();i++)
  {
    if((temp=distance(x,y,vect[i].x(),vect[i].y()))<dis)
    {
      dis=temp;
      index=i;
    }
  }
  return index;
}

int nearestLoc(vector<location> vect, int x, int y)
{
  if(vect.size()==0) return -1;
  
  int index=0,dis=99999,temp;
  for(unsigned int i=0;dis>0 && i<vect.size();i++)
  {
    if((temp=distance(x,y,vect[i].x,vect[i].y))<dis)
    {
      dis=temp;
      index=i;
    }
  }
  //std::cout<<"Index: "<<index<<std::endl;
  return int(index);
}

template <class T>
int nearest(vector<T> vect, location loc)
{
  return nearest(vect,loc.x,loc.y);
}

template <class T>
int lowestHP(vector<T> vect)
{
  if(vect.size()==0) return -1;
  int returnVal = 0;
  for(unsigned int i=1;i<vect.size();i++)
  {
    if(vect[i].hp() < vect[returnVal].hp() && vect[i].hp() > 0)
    {
      returnVal = i;
    }
  }
  return int(returnVal);
}
#include <iostream>

class AI: public BaseAI
{
public:
  AI();
  void init(){};
  vector<location> zombieSightLocations();
  vector<location> zombieAttackLocations();
  location meshWalls(int x, int y);  
  vector<location> validMoves(int x, int y);
  
  vector<location> withinRange(int x, int y,int range);
  vector<Zombie> zwithinRange(int x, int y,int range);
  
  void nearSpawn(int number,location estimate);
  location getEstimate();

  
  void DOMANSTAR(Zombie& THEMAN, short filling[27][27]);
  bool inBounds(int x, int y);
  
  virtual const char* username();
  virtual const char* password();
  virtual bool runHuman();
  virtual bool runZombie();
};

#endif
