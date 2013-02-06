#ifndef AI_H
#define AI_H

#include "baseAI.h"
#include <iostream>
#include <ctime>
#include <cmath>
#include <deque>
#include <list>
#include <map>

class ZombieStep;

class AI: public BaseAI
{
public:
  AI(); //{srand(time(NULL));};
  //~AI();
  const char* username(){ return "MuesAI";}
  const char* password(){ return "x7b0fw12abslkfgs8234nsdfluya097aweo3";}
  virtual bool runHuman();
  virtual bool runZombie();
  void blindZombie(int i);
  void smartZombie(int i);
  void obedientZombie(int index, std::deque<int> myOrders);
  void humanAI();
  void builderHuman(int i);
  void scaredHuman(int i);
  void zombieAI();
  void placeZombies();
  void placeHumans();
  void callAirstrikes();
  static int nextY(int x, int y, int facing);
  static int nextX(int x, int y, int facing);
  int getWall(int x, int y);
  int getHuman(int x, int y);
  int getZombie(int x, int y);
  int getCrate(int x, int y);

  int eaten; //How many zombies were eaten this turn.

  int distance(int startX, int startY, int targetX, int targetY);
  int zombieDistance(int startX, int startY, int targetX, int targetY,
                     int facing);
  bool isFacing(int startX, int startY, int targetX, int targetY,
                  int facing);

  int min(int a, int b);
  int max(int a, int b);

  std::deque<int> findZombiePath(int startX, int startY, int targetX,
                                 int targetY, int facing);

  std::map<int, std::deque<int> > orders;

  std::map<int, int> wallMap; //(mapKey(x,y), index in list)
  void updateWallMap();
  int mapKey(int x, int y);

};

#include <deque>
#include <iostream>

class ZombieStep
{
public:
  ZombieStep(ZombieStep* original);
  ZombieStep(int newX, int newY, int newFacing, ZombieStep* newParent);
  ZombieStep(ZombieStep* newParent, int action);
  void doAction(int action);

  int x;
  int y;
  int facing;
  int action;
  int depth;
  int remaining;
  bool operator<(const ZombieStep & rhs);
  bool operator==(const ZombieStep & rhs);
  ZombieStep* parent;
  int mapKey();
};


//std::deque<ZombieStep> findZombiePath(AIint startX, int startY, int targetX,
//                                 int targetY, int facing);

void displayPath(std::deque<int> path);

bool SortProcess(ZombieStep* left, ZombieStep* right)
{
  if((left->depth + left->remaining) < (right->depth + right->remaining))
  //if((left->depth) < (right->depth))
  {
    return true;
  }
  else
  {
    return false;
  }
};


#endif
