#include "AI.h"
const char* AI::username()
{
  return "Ghost AI";
}

const char* AI::password()
{
  return "ROBOTHOUSE";
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
  cList.resize(15);
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
      <<"\tDamage  :"<<weapon.delay()<<endl
      <<"\tVSplashD:"<<weapon.verticalSplashDamage()<<endl
      <<"\tHSplashD:"<<weapon.horizontalSplashDamage()<<endl
      <<"\tHSplashR:"<<weapon.horizontalSplashRadius()<<endl;
}

bool AI::runHuman()
{
  if(turnNum() == 1)
  {
    for(unsigned int w=0;w<weapons.size();w++)
    {
      cout<<"WeaponID: "<<w<<endl;
      displayWeapon(weapons[w]);          
    }
    vector<location> nearby = getAdjacent(0,0);
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
      int pick = rand()%nearby.size();
      humans[i].move(nearby[pick].x,nearby[pick].y);
    }
  }

  return true;
}

bool AI::runZombie()
{
  cout<<turnNum()<<endl;
  int freeToSpawn = (getZombieCap() - zombies.size()<getZombiesReady()?getZombieCap() - zombies.size():getZombiesReady());
  //cout<<"Cap: "<<getZombieCap()<<" Size: "<<zombies.size()<<" Ready: "<<getZombiesReady()<<" Spawning: "<<freeToSpawn<<endl;
  for(int s=0;s<freeToSpawn;s++)
  {
    Zombie::spawn(spawnzones[rand()%spawnzones.size()],rand()%6);
  }
  int index;
  for(unsigned int z=0;z<zombies.size();z++)
  {
    //For each Zombie, A* to random location for now.
    //cout<<"Uh, before zombie's pathing"<<endl;
	for(int i = 0; i < cList.size(); i++)
	{
	  cList[i].clear();
	}
	vector<short> path;
	Zlocation current = Zlocation(zombies[z].x(),zombies[z].y(),zombies[z].facing());
	//cout<<"Zomb cur loc:  "<<zombies[z].x()<<"  "<<zombies[z].y()<<"  "<<zombies[z].facing()<<endl;
	path = aStarSearch(current, Zlocation(1, 1, 3));
	//cout<<"After Zombie Pathing"<<endl;
	if(path.size() >0)
	{
	  if(path[0] == 1)
	  {
        zombies[z].turn(TURN_LEFT);
	  }
	  else if(path[0] == -1)
	  {
        zombies[z].turn(TURN_RIGHT);
	  }
	  else
	  {
        zombies[z].move();
	  }
	}
		  
	//Do move based on Path.
  }
  return true;
}

//--------------------------------------------------A* CRAP
bool operator<(const Zwrapper& rhs, const Zwrapper& lhs)
{
  return(lhs.hX+lhs.gX > rhs.hX+rhs.gX);
}
bool operator==(const Zwrapper& rhs, const Zwrapper& lhs)
{
  if(lhs.node==rhs.node)
  {
    return true;
  }
  return false;
}
bool operator>(const Zwrapper& rhs, const Zwrapper& lhs)
{
  return(lhs.hX+lhs.gX < rhs.hX+rhs.gX);
}

Zwrapper::Zwrapper(Zlocation blah, short H, short G, short PREV, vector< short > movehist)
{
  node.x = blah.x;
  node.y = blah.y;
  node.face = blah.face;
  hX = H;
  gX = G;
  movehistory = movehist;
  movehistory.push_back(PREV);  
}

bool AI::findmoves(Zlocation current)
{
  location myLoc(current.x,current.y);
  location inFrontOfMe = myLoc + offset[current.face];  
  int index;
  
  if(!inBounds(inFrontOfMe.x,inFrontOfMe.y))
  {
  }
  //if there is a wall in front of you
  else if((index=atLocation(walls,inFrontOfMe))!=-1)
  {
  }
  //if a human is directly in front of you
  else if((index=atLocation(humans,inFrontOfMe)) != -1)
  {
  }
  else //if nothing is directly in front of you
  {
	return true;
  }
  return false;
}

bool AI::pushZ(Zwrapper toPush)
{
  //Check the closed list.
  int Xhash = toPush.node.x;
  Xhash = (Xhash<0)?(-Xhash):(Xhash);
  for(int i = 0; i < cList[Xhash].size(); i++)
  {
	  if(toPush.node.x == cList[Xhash][i].node.x && toPush.node.y == cList[Xhash][i].node.y && toPush.node.face == cList[Xhash][i].node.face )
	  {
		  //It's in the list, return false
		  return false;
	  }
  }
  //If not there, add it and return true.
  cList[Xhash].push_back(toPush);
  return true;
}

vector<short> AI::aStarSearch(Zlocation current, Zlocation goal)
{
  bool goalFound, forward;
  char junk;
  vector<short> solution;
  vector<short> currenthist;
  priority_queue<Zwrapper> oList;
  Zwrapper temp;
  //Find all moves that are not redundant, make children of them.
  forward =findmoves(current);
  //Turn Left
  temp = Zwrapper(Zlocation(current.x, current.y, (current.face+1)%6), 
	       distance(current.x, current.y,goal.x,goal.y), 1, 1, currenthist);
  if(pushZ(temp))
  {
    oList.push(temp);
  }
  //Turn Right
  if(current.face == 0)
  {
    temp = Zwrapper(Zlocation(current.x, current.y, 5), 
            distance(current.x,current.y,goal.x,goal.y), 1, -1, currenthist);
	if(pushZ(temp))
	{
      oList.push(temp);
	}
  }
  else
  {
    temp = Zwrapper(Zlocation(current.x, current.y, current.face-1), 
            distance(current.x,current.y,goal.x,goal.y), 1, -1, currenthist);
	if(pushZ(temp))
	{
      oList.push(temp);
	}
  }
  //Forward
  if(forward)
  {
    location myLoc(current.x,current.y);
    location inFrontOfMe = myLoc + offset[current.face];
	temp = Zwrapper(Zlocation(inFrontOfMe.x,inFrontOfMe.y, current.face), 
            distance(current.x,current.y,goal.x,goal.y), 1, 0, currenthist);
	if(pushZ(temp))
	{
      oList.push(temp);
	}
  }
  while(!goalFound)
  {
	currenthist = oList.top().movehistory;
    if(oList.top().node.x==goal.x && oList.top().node.y==goal.y)
    {
	  //Iterate this shit in traceback.

	  solution = currenthist;
	  goalFound = true;
	}
	else
	{
	//If NOT, push it's children into the trace.
	  forward =findmoves(oList.top().node);
	//PUSH LEETLE CHILDREN

	  //----------------------------ATTEMPTING FIX
  //Turn Left
  temp = Zwrapper(Zlocation(oList.top().node.x, oList.top().node.y, (oList.top().node.face+1)%6), 
	       distance(oList.top().node.x, oList.top().node.y,goal.x,goal.y), oList.top().gX+1, 1, currenthist);
  if(pushZ(temp))
  {
    oList.push(temp);
  }
  //Turn Right
  if(oList.top().node.face == 0)
  {
    temp = Zwrapper(Zlocation(oList.top().node.x, oList.top().node.y, 5), 
            distance(oList.top().node.x,oList.top().node.y,goal.x,goal.y), oList.top().gX+1, -1, currenthist);
	if(pushZ(temp))
	{
      oList.push(temp);
	}
  }
  else
  {
    temp = Zwrapper(Zlocation(oList.top().node.x, oList.top().node.y, oList.top().node.face-1), 
            distance(oList.top().node.x,oList.top().node.y,goal.x,goal.y), oList.top().gX+1, -1, currenthist);
	if(pushZ(temp))
	{
      oList.push(temp);
	}
  }
  //Forward
  if(forward)
  {
    location myLoc(oList.top().node.x,oList.top().node.y);
    location inFrontOfMe = myLoc + offset[oList.top().node.face];
	temp = Zwrapper(Zlocation(inFrontOfMe.x,inFrontOfMe.y, oList.top().node.face), 
            distance(inFrontOfMe.x,inFrontOfMe.y,goal.x,goal.y), oList.top().gX+1, 0, currenthist);
	if(pushZ(temp))
	{
      oList.push(temp);
	}
  }

//----------------------------ATTEMPTING FIX
	}
	if(oList.size() > 0)
	{
	  oList.pop();
	}
	else
	{
		goalFound = true;
	}
  }
  //If we're here, goal is found and solution carries the way there.
  return solution;
}

//------------------------------------------


