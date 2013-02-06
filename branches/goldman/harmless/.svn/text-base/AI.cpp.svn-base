#include "AI.h"
const char* AI::username()
{
  return "Harmless";
}

const char* AI::password()
{
  return "mostly";
}

#define UP 2
#define UP_RIGHT 1
#define DOWN_RIGHT 0
#define DOWN 5
#define DOWN_LEFT 4
#define UP_LEFT 3

#define TURN_LEFT 1
#define TURN_RIGHT -1

location offset[] = {
                     location( 1, 0),//0
                     location( 0, 1),//1
                     location(-1, 1),//2
                     location(-1, 0),//3
                     location( 0,-1),//4
                     location( 1,-1) //5
                     };
                     
                     
AI::AI()
{
  srand(time(NULL));
}

//returns a vector of locations around a location
vector<location> getAdjacent(location myLoc)
{
  vector<location> returnVector(6);
  
  for(unsigned int o=0;o<6;o++)
  {
    returnVector[o] = myLoc + offset[o];
  }
  return returnVector;
}

//returns a vector of locations around a location
vector<location> getAdjacent(int x, int y)
{
  return getAdjacent(location(x,y));
}

bool AI::inBounds(int x, int y)
{
  int radius = getMapSize();
  return !(x<-radius || x>radius || y<-radius || y>radius);
}

//returns a vector of only valid moves
vector<location> AI::validMoves(int x, int y)
{
  bool valid[] = {true,true,true,true,true,true};
  int stillValid=6;
  for(unsigned int o=0;o<6;o++)
  {
    if(!inBounds(x+offset[o].x,y+offset[o].y))
    {
      valid[o] = false;
      stillValid--;
    }
    for(unsigned int w=0;valid[o] && w<walls.size();w++)
    {
      if(walls[w].x()==x+offset[o].x && walls[w].y()==y+offset[o].y)
      {
        valid[o]=false;
        stillValid--;
      }
    }
    for(unsigned int z=0;valid[o] && z<zombies.size();z++)
    {
      if(zombies[z].x()==x+offset[o].x && zombies[z].y()==y+offset[o].y)
      {
        valid[o]=false;
        stillValid--;
      }
    }
    for(unsigned int h=0;valid[o] && h<humans.size();h++)
    {
      if(humans[h].x()==x+offset[o].x && humans[h].y()==y+offset[o].y)
      {
        valid[o]=false;
        stillValid--;
      }
    }
  }
  location myLocation(x,y);
  vector<location> returnVector(stillValid);
  for(unsigned int o=0,i=0;o<6;o++)
  {
    if(valid[o])
    {
      returnVector[i] = myLocation+offset[o];
      i++;
    }
  }
  return returnVector;
}

int AI::distance(int startX, int startY, int targetX, int targetY)
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

vector<location> AI::withinRange(int x, int y,int range)
{
  vector<location> inRange;
  for(unsigned int z=0;z<zombies.size();z++)
  {
    if(distance(x,y,zombies[z].x(),zombies[z].y())< range)
    {
      inRange.push_back(location(zombies[z].x(),zombies[z].y()));
    }
  }
  return inRange;
}


void displayWeapon(Weapon weapon)
{
  cout<<"\tName    :"<<weapon.name()<<endl
      <<"\tRange   :"<<weapon.range()<<endl
      <<"\tAmmo    :"<<weapon.ammo()<<endl
      <<"\tDelay   :"<<weapon.delay()<<endl
      <<"\tDamage  :"<<weapon.damage()<<endl
      <<"\tVSplashD:"<<weapon.verticalSplashDamage()<<endl
      <<"\tHSplashD:"<<weapon.horizontalSplashDamage()<<endl
      <<"\tHSplashR:"<<weapon.horizontalSplashRadius()<<endl;
}

bool AI::runHuman()
{
  cout<<"Human Turn: "<<turnNum()<<endl;
  if(turnNum() == 1)
  {
    for(unsigned int w=0;w<weapons.size();w++)
    {
      cout<<"WeaponID: "<<w<<endl;
      displayWeapon(weapons[w]);          
    }
    vector<location> nearby = validMoves(0,0);
    while(nearby.size()<5)
    {
      //should keep trying to find 6 valid spots at random on the map
      nearby = validMoves(rand()%(getMapSize()*2)-getMapSize(),rand()%(getMapSize()*2)-getMapSize());
    }
    for(int i=0;i<getHumansReady();i++)
    {
      Human::spawn(weapons[i], nearby[i].x, nearby[i].y);
    }
  }
  if(turnNum() == 3)
  {
    for(unsigned int i=0;i<humans.size();i++)    
    {
      cout<<"Human["<<humans[i].id()<<"] is carrying weapon "<<humans[i].weaponID()<<endl;
      displayWeapon(weapons[humans[i].weaponID()]);
    }
  }
  
  for(unsigned int i=0;i<humans.size();i++)
  {
    for(unsigned int m=0;m<humans[i].moves();m++)
    {
      vector<location> nearby = validMoves(humans[i].x(),humans[i].y());
      if(nearby.size()>0)
      {
        int pick = rand()%nearby.size();
        humans[i].move(nearby[pick].x,nearby[pick].y);
      }
      else
      {
        cout<<"Human["<<humans[i].id()<<"] is completely surrounded"<<endl;
      }
    }
  }
  cout<<"End Turn"<<endl;
  return true;
}

bool AI::runZombie()
{
  cout<<"Zombie Turn: "<<turnNum()<<endl;
  int freeToSpawn = (getZombieCap() - zombies.size()<getZombiesReady()?getZombieCap() - zombies.size():getZombiesReady());
  //cout<<"Cap: "<<getZombieCap()<<" Size: "<<zombies.size()<<" Ready: "<<getZombiesReady()<<" Spawning: "<<freeToSpawn<<endl;
  for(int s=0;s<freeToSpawn;s++)
  {
    Zombie::spawn(spawnzones[rand()%spawnzones.size()],rand()%6);
  }
  int index;
  for(unsigned int z=0;z<zombies.size();z++)
  {
    location myLoc(zombies[z].x(),zombies[z].y());
    location inFrontOfMe = myLoc + offset[zombies[z].facing()];
    if(!inBounds(inFrontOfMe.x,inFrontOfMe.y))
    {
      zombies[z].turn( (rand()%2==0?TURN_LEFT:TURN_RIGHT) );
    }
    //if there is a wall in front of you
    else if((index=atLocation(walls,inFrontOfMe))!=-1)
    {
//      cout<<"Zombie Facing Wall["<<index<<"] at "<<walls[index].x()<<" "<<walls[index].y()<<endl;
      //turns at random either left or right
      zombies[z].turn( (rand()%2==0?TURN_LEFT:TURN_RIGHT) );
    }
    //if a human is directly in front of you
    else if((index=atLocation(humans,inFrontOfMe)) != -1)
    {
      zombies[z].turn( (rand()%2==0?TURN_LEFT:TURN_RIGHT) );
    }
    else //if nothing is directly in front of you
    {
      zombies[z].move();
    }
  }
  cout<<"EndTurn"<<endl;
  return true;
}
