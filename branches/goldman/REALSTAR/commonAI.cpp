#include "AI.h"



const char* AI::username()
{
  return "Shell AI";
}

const char* AI::password()
{
  return "password";
}

void AI::init()
{
  srand(time(NULL));
}


vector< vector < mpair > > AI::getBlocking()
{
  vector <vector <mpair> > returnVec(31, vector<mpair> (31));
  for(unsigned int i=0;i<walls.size();i++)
  {
    returnVec[walls[i].x()+15][walls[i].y()+15] =  mpair(WALL,i) ;
  }
  for(unsigned int i=0;i<zombies.size();i++)
  {
    returnVec[zombies[i].x()+15][zombies[i].y()+15] = mpair(ZOMBIE,i);      
  }
  for(unsigned int i=0;i<humans.size();i++)
  {
    returnVec[humans[i].x()+15][humans[i].y()+15] = mpair(HUMAN,i);
  }
  for(unsigned int i=0;i<crates.size();i++)
  {
    returnVec[crates[i].x()+15][crates[i].y()+15] = mpair(CRATE,i);
  }
  return returnVec;  
}

vector< vector < mpair > > AI::getDanger()
{
  vector <vector <mpair> > returnVec(31, vector<mpair> (31));
  for(unsigned int i=0;i<zombies.size();i++)
  {
    returnVec[zombies[i].x()+15+offset[zombies[i].facing()].x][zombies[i].y()+15+offset[zombies[i].facing()].y].type=ZATTACK;
    returnVec[zombies[i].x()+15+offset[zombies[i].facing()].x][zombies[i].y()+15+offset[zombies[i].facing()].y].value+=5;
  }
  for(unsigned int i=0;i<zombies.size();i++)
  {
    //zombie sight
    int delta = 2;
    for( int x=-delta;x<=delta;x++)
    {
      for( int y=-delta;y<=delta;y++)
      {
        if(distance(0,0,x,y) <= delta)
        {
          if(returnVec[zombies[i].x()+15+x][zombies[i].y()+15+y].type!=ZATTACK)
          {
            returnVec[zombies[i].x()+15+x][zombies[i].y()+15+y].type = ZSIGHT;
            returnVec[zombies[i].x()+15+x][zombies[i].y()+15+y].value+=1;
          }
        }      
      }
    }
  }
  return returnVec;
}
