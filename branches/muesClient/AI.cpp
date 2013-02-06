#include "AI.h"


AI::AI()
{
  srand(time(NULL));
}


/*
AI::~AI()
{

}
*/


//Straight line distance
//Could be used as an A* heuristic for a path finding function
int AI::distance(int startX, int startY, int targetX, int targetY)
{
  int dis;
  int dx = targetX - startX;
  int dy = targetY - startY;
  if (dx * dy > 0)
    dis = abs(dx) + abs(dy);
  else
    dis = max(abs(dx), abs(dy));
  return dis;
}

//Straight line distance + minimum rotations required
//Could be used as a better A* heuristic for the 
//   ZOMBIE's path finding function
int AI::zombieDistance(int startX, int startY, int targetX, int targetY,
                       int facing)
{
  bool thisWay;
  int initRotations = 6; //rotations needed before moving
  int otherRotations = 0; //rotations needed later
  int dis = distance(startX, startY, targetX, targetY);

  int rotNeeded; //rotations needed to meet the current facing.

  for (int i = 0; i < 6; i++)
  {
    thisWay = isFacing(startX, startY, targetX, targetY, i);

    if (thisWay)
    {
      rotNeeded = abs(facing - i);
      rotNeeded = min(abs(facing - i - 6), rotNeeded);
      rotNeeded = min(abs(facing - i + 6), rotNeeded);
      initRotations = min(initRotations, rotNeeded);

      if (facing != i)
      {
        otherRotations += 1;
      }
    }
  }
  if (initRotations > 0)
    otherRotations -= 1;

  if (dis > 0)
    dis = dis + initRotations + otherRotations;  
  return dis;
}

int AI::min(int a, int b)
{
  return a<b?a:b;
}

int AI::max(int a, int b)
{
  return a>b?a:b;
}

bool AI::isFacing(int startX, int startY, int targetX, int targetY, 
                  int facing)
{
  bool inFront = false;
  if (facing == 0)
    inFront = (targetX > startX 
               && targetY > startX - targetX + startY);
  else if (facing == 1)
    inFront = (targetY > startX - targetX + startY
               && targetY > startY);
  else if (facing == 2)
    inFront = (targetY > startY && targetX < startX);
  else if (facing == 3)
    inFront = (targetX < startX
               && targetY < startX - targetX + startY);
  else if (facing == 4)
    inFront = (targetY < startX - targetX + startY
               && targetY < startY);
  else if (facing == 5)
    inFront = (targetY < startY && targetX > startX);
  return inFront;
}

//Returns a deque of zombie steps to get you to the target.
//facing is the current facing
//Uses A* hopefully.
std::deque<int> AI::findZombiePath(int startX, int startY, int targetX,
                                 int targetY, int facing)
{
  std::cout << "s" << std::endl;
  const int ZOMBIE_ATTACK = 5 * 5;
  int wallIndex;
  bool solved = false;
  bool addChild;
  std::deque<int> solution;
  std::list<ZombieStep*> openList;
  std::map<int, ZombieStep*> allStates; //(ZombieStep.mapKey(), ZombieStep)
  std::map<int, ZombieStep*>::iterator iter;
  ZombieStep* start = new ZombieStep(startX, startY, facing, NULL);
  ZombieStep* nextChild;
  ZombieStep* parent;

  openList.push_front(start);
  allStates[start->mapKey()] = start;

  while (openList.size() > 0 && !solved)
  {
    solved = (openList.front()->x == targetX)
             && (openList.front()->y == targetY);
    
    if (!solved)
    {
      for (int i = 0; i < 3; i++)
      {
        nextChild = new ZombieStep(openList.front(), i);

        wallIndex = getWall(nextChild->x, nextChild->y);
        if (wallIndex > -1)
        {
          nextChild->depth += walls[wallIndex].hp() / ZOMBIE_ATTACK;
        }

        std::map<int, ZombieStep*>::iterator iter;
        int index = -1;
        iter = allStates.find(nextChild->mapKey());
        if (iter != allStates.end())
          addChild = false;
        else
          addChild = true;

        if (nextChild->x > getMapSize() || nextChild->x < -getMapSize())
          addChild = false;
        else if (nextChild->y > getMapSize() || nextChild->y < -getMapSize())
          addChild = false;
        
        if (addChild)
        {
          nextChild->remaining = zombieDistance(nextChild->x, nextChild->y,
                                    targetX, targetY, nextChild->facing);
          openList.push_back(nextChild);
          allStates[nextChild->mapKey()] = nextChild;
        }
        else
        {
          delete nextChild; //free if not added
        }
      }
      openList.pop_front();
    }
  }

  solution.clear();
  if (solved)
  {
    parent = openList.front();
    while(parent != NULL)
    {
      solution.push_front(parent->action);
      parent = parent->parent;
    }
  }
  

  for(iter = allStates.begin(); iter != allStates.end(); ++iter )
  {
    delete (iter->second);
  }
  return solution;
}


bool AI::runHuman()
{
  int attacks = 1;
  int moves;
  int tries;
  int dir;
  int x, y;
  int oldX, oldY;
  int zombieIndex, crateIndex;

  for (int i = 0; i < weapons.size(); i++)
  {
    std::cout << weapons[i].name() << std::endl;
    std::cout << weapons[i].canSpawnWith() << std::endl;
    std::cout << weapons[i].canCall()  << std::endl;
  }
  //while (1){};
    
  

  std::cout << getHumanTeamName() << std::endl;
  std::cout << getHumanTime() << std::endl;
  std::cout << getZombieTeamName() << std::endl;
  std::cout << getZombieTime() << std::endl;
  std::cout << getMapSize() << std::endl;

  updateWallMap();
  placeHumans();
  
  for (int i = 0; i < humans.size(); i++)
  {
    if (humans[i].id() % 2 == 1)
    {
      scaredHuman(i);
    }
    else
    {
      builderHuman(i);
    }
  }

  callAirstrikes();
  return true;
}

//human i runs around randomly and attacks things if they get in the way
void AI::scaredHuman(int i)
{
  int attacks = 1;
  int moves;
  int tries;
  int dir;
  int x, y;
  int oldX, oldY;
  int zombieIndex, crateIndex;

  tries = 0;
  moves = humans[i].moves();
  oldX = humans[i].x();
  oldY = humans[i].y();
  while ((moves > 0 || attacks > 0) && tries < 12)
  {
    dir = rand() % 6;
    x = nextX(oldX, oldY, dir);
    y = nextY(oldX, oldY, dir);
    zombieIndex = getZombie(x, y);
    crateIndex = getCrate(x, y);
    if (crateIndex > -1)
    {
      humans[i].grab(crates[crateIndex]);
    }

    if (zombieIndex == -1 && getWall(x, y) == -1 && moves > 0)
    {
      humans[i].move(x, y);
      oldX = x;
      oldY = y;
      moves -= 1;
    }
    else if (zombieIndex > -1 && attacks > 0)
    {
      humans[i].attack(zombies[zombieIndex]);
      attacks -= 1;
    }
    tries += 1;
  }
}

//Attacks nearby zombies, then builds walls around himself.
//Does not move.
void AI::builderHuman(int i)
{
  int attacks = 1;
  int tries = 0;
  int dir;
  int x, y;
  int zombieIndex, crateIndex, humanIndex, wallIndex;
  int moves = humans[i].moves();

  while ((moves > 0 || attacks > 0) && tries < 12)
  {
    dir = rand() % 6;
    x = nextX(humans[i].x(), humans[i].y(), dir);
    y = nextY(humans[i].x(), humans[i].y(), dir);
    zombieIndex = getZombie(x, y);
    crateIndex = getCrate(x, y);
    humanIndex = getHuman(x, y);
    wallIndex = getHuman(x, y);

    if (crateIndex > -1)
    {
      humans[i].grab(crates[crateIndex]);
    }

    if (zombieIndex == -1 && humanIndex == -1 && moves > 0)
    {
      humans[i].build(x, y);
      moves -= 1;
    }
    else if (zombieIndex > -1 && attacks > 0)
    {
      humans[i].attack(zombies[zombieIndex]);
      attacks -= 1;
    }
    tries += 1;
  }
}

void AI::placeHumans()
{
  int x, y, count;
  count = getHumansReady();
  while (count > 0)
  {
    x = rand() % 7 - 3;
    y = rand() % 7 - 3;
    if (getWall(x,y) == -1)
    {
      count -= 1;
      Human::spawn(weapons[4], x, y);
    }
  }
}

void AI::callAirstrikes()
{
  int calls = int(getAirstrikesReady());
  int x, y;

  for (int i = 0; i < calls; i++)
  {
    do
    {
      x = (rand()%15) - 7;
      y = (rand()%15) - 7;
    } while (getHuman(x, y) > -1);
    Human::callAmmoDrop(x, y);
  }
}

void AI::updateWallMap()
{
  wallMap.clear();
  for (int j = 0; j < walls.size(); j++)
  {
    wallMap[mapKey(walls[j].x(), walls[j].y())] = j;
  }
}

int AI::mapKey(int x, int y)
{
  int key;
  const int Y_MULTIPLIER = 100000;
  key = x + y * Y_MULTIPLIER;
  return key;
}

int AI::getWall(int x, int y)
{
  std::map<int, int>::iterator iter;
  int index = -1;
  iter = wallMap.find(mapKey(x, y));
  if (iter != wallMap.end())
    index = iter->second;
  return index;
}

int AI::getHuman(int x, int y)
{
  int index = -1;
  for (int j = 0; j < humans.size(); j++)
  {
    if (humans[j].y() == y && humans[j].x() == x)
      index = j;
  }
  return index;
}

int AI::getZombie(int x, int y)
{
  int index = -1;
  for (int j = 0; j < zombies.size(); j++)
  {
    if (zombies[j].y() == y && zombies[j].x() == x)
      index = j;
  }
  return index;
}

int AI::getCrate(int x, int y)
{
  int index = -1;
  for (int j = 0; j < crates.size(); j++)
  {
    if (crates[j].y() == y && crates[j].x() == x)
      index = j;
  }
  return index;
}


bool AI::runZombie()
{
  std::map<int, std::deque<int> >::iterator iter;
  eaten = 0;
  updateWallMap();
  for (int i = 0; i < zombies.size(); i++)
  {
    if (getTurnNumber()<5 || humans.size() == 0)
      blindZombie(i);
    else
    {
      smartZombie(i);
    }
  }
  placeZombies();
  return true;
}


//Zombie i is given orders based on an A* search to the nearest human
//   where nearest is defined by the zombieDistance function
//Smart zombies will not break down walls, and will do nothing if
//   no path is available to the nearest human.
void AI::smartZombie(int index)
{
  const int FORGET = 5; //forget the last x of the generated path.
  const int REMEMBER = 5; //go no more than x turns without recalulating path
  int forgotten = 0;
  Zombie* me = &zombies[index];
  Human* target = NULL;
  int bestDis = 9999999;
  int nextDis;
  int x, y, humanIndex, wallIndex;
  std::deque<int> path;
  std::cout << "smart" << std::endl;
  for (int i = 0; i < humans.size(); i++)
  {
    nextDis = zombieDistance(me->x(), me->y(), humans[i].x(), humans[i].y(), 
                             me->facing());
    if (nextDis < bestDis)
    {
      bestDis = nextDis;
      target = &humans[i];
    }
  }
  
  if (target != NULL)
  {
    path = findZombiePath(me->x(), me->y(), target->x(), target->y(),
                          me->facing());
      
    std::cout << "(" << me->x() << "," <<  me->y() << "," << target->x()
         << "," <<  target->y() << "," << me->facing() << ") = ";
    for (int k = 0; k < path.size(); k++)
    {
      std::cout << path[k] << " ";
    }
    if (path.size() == 0)
    {
      std::cout << "No action";
    }
    std::cout << std::endl;

    if (path.size() > 1)
    {
      path.pop_front();
      switch (path.front())
      {
      case 0:
        x = nextX(me->x(), me->y(), me->facing());
        y = nextY(me->x(), me->y(), me->facing());
        humanIndex = getHuman(x, y);
        wallIndex = getWall(x, y);
        if (humanIndex > -1)
          me->attack(humans[humanIndex]);
        else if (wallIndex > -1)
          me->attack(walls[wallIndex]);
        else
          me->move();
        break;
      case 1:
        me->turn(-1);
        break;
      case 2:
        me->turn(1);
        break;
      }
      /*
      while (path.size() > 0 && forgotten < FORGET)
      {
        path.pop_back();
        forgotten += 1;
      }
      while (path.size() > REMEMBER)
      {
        path.pop_back();
      } 
      if (path.size() > 0)
        orders[me->id()] = path;
      */
    }
  }
  std::cout << "end" << std::endl;
}

void AI::obedientZombie(int index, std::deque<int> myOrders)
{
  int x, y, humanIndex, wallIndex;
  Zombie* me = &zombies[index];

  switch (myOrders.front())
  {
  case 0:
    x = nextX(me->x(), me->y(), me->facing());
    y = nextY(me->x(), me->y(), me->facing());
    humanIndex = getHuman(x, y);
    if (humanIndex > -1)
    {
      myOrders.clear();
      me->attack(humans[humanIndex]);
    }
    else
      me->move();
    break;
  case 1:
    me->turn(-1);
    break;
  case 2:
    me->turn(1);
    break;
  }

  if (myOrders.size() > 0)
    myOrders.pop_front();
  if (myOrders.size() > 0)
    orders[me->id()] = myOrders;
  else
    orders.erase(me->id());
  
}


//Zombie i is given orders based soley on its smell and the object
//   directly in front of them.
void AI::blindZombie(int i)
{
  int x, y, wallIndex, humanIndex, zombieIndex;
  bool allowEat = (int(getZombieCap()) == zombies.size());

  if (zombies[i].smell() == 0)
  {
    zombies[i].turn((rand()%2) * 2 - 1);
  }
  else
  {
    x = nextX(zombies[i].x(), zombies[i].y(), zombies[i].facing());
    y = nextY(zombies[i].x(), zombies[i].y(), zombies[i].facing());
    wallIndex = getWall(x, y);
    humanIndex = getHuman(x, y);
    zombieIndex = getZombie(x, y);
 
    if (humanIndex > -1)
    {
      zombies[i].attack(humans[humanIndex]);
    }
    else if (wallIndex > -1)
    {
      zombies[i].attack(walls[wallIndex]);
    }
    else if (zombieIndex > -1 && allowEat)
    {
      zombies[i].eat(zombies[zombieIndex]);
      eaten += 1;
    }
    else
    {
      zombies[i].move();
    }
  }
}

void AI::placeZombies()
{
  int available = int(getZombieCap()) - zombies.size() + eaten;
  int zoneIndex;
  int facing;
  available = (available < getZombiesReady() ? available:getZombiesReady());

  for (int i = 0; i < available; i++)
  {
    zoneIndex = rand() % 4;
    switch (zoneIndex)
    {
    case 0:
      facing = 0;
      break;
    case 1:
      facing = 4;
      break;
    case 2:
      facing = 3;
      break;
    case 3:
      facing = 1;
      break;
    }
    Zombie::spawn(spawnzones[zoneIndex], facing);
    available -= 1;
  }
}

int AI::nextX(int x, int y, int facing)
{
  //Utility function for determining the X coordinate of
  //   the hexagon immediately in front of the zombie.
  int retVal;
  if ((facing % 5) == 0)
    retVal = x + 1;
  else if (((facing - 1) % 3) == 0)
    retVal = x;
  else
    retVal = x - 1;
  return retVal;
}

int AI::nextY(int x, int y, int facing)
{
  //Utility function for determining the X coordinate of
  //the hexagon immediately in front of the zombie.
  int retVal;
  if (facing >= 4)
    retVal = y - 1;
  else if ((facing % 3) == 0)
    retVal = y;
  else
    retVal = y + 1;
  return retVal;
}

ZombieStep::ZombieStep(ZombieStep* original)
{
  x = original->x;
  y = original->y;
  facing = original->facing;
  action = original->action;
  parent = original->parent;
  depth = original->depth;
}

ZombieStep::ZombieStep(int newX, int newY, int newFacing, ZombieStep* newParent)
{
  x = newX;
  y = newY;
  facing = newFacing;
  parent = newParent;
  action = -1;
  depth = 0;
}

ZombieStep::ZombieStep(ZombieStep* original, int newAction)
{
  x = original->x;
  y = original->y;
  facing = original->facing;
  parent = original;
  depth = original->depth;
  //std::cout << "action=" << action << std::endl;
  doAction(newAction);
}

void ZombieStep::doAction(int newAction)
{
  action = newAction;
  depth += 1;
  switch (action)
  {
  case 0:
    x = AI::nextX(x, y, facing);
    y = AI::nextY(x, y, facing);
    break;
  case 1:
    facing -= 1;
    if (facing < 0)
      facing += 6;
    break;
  case 2:
    facing += 1;
    if (facing > 5)
      facing -= 6;
    break;
  }
}

bool ZombieStep::operator<(const ZombieStep& rhs)
{
  return ((depth + remaining) < (rhs.depth + rhs.remaining));
};

bool ZombieStep::operator==(const ZombieStep& rhs)
{
  return (x == rhs.x && y == rhs.y && facing == rhs.facing);
}

void displayPath(std::deque<int> path)
{
  for (int k = 0; k < path.size(); k++)
  {
    std::cout << path[k] << ",";
  }
  if (path.size() == 0)
  {
    std::cout << "No action";
  }
  std::cout << std::endl;
}


int ZombieStep::mapKey()
{
  const int Y_MULT = 1000000;
  const int X_MULT = 10;
  int key = x * X_MULT + y * Y_MULT + facing;
}

