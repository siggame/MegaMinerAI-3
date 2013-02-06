#include "AI.h"
const char* AI::username()
{
  return "Begin AI";
}

const char* AI::password()
{
  return "easy";
}

#define DOWN 5
#define DOWN_LEFT 4
#define UP_LEFT 3
#define UP 2
#define UP_RIGHT 1
#define DOWN_RIGHT 0


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

bool AI::inBounds(int x, int y)
{
  int radius = getMapSize();
  return !(x<-radius || x>radius || y<-radius || y>radius);
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


vector<location> AI::zombieAttackLocations()
{
  vector<location> returnVec(zombies.size());
  for(unsigned int z=0; z<zombies.size();z++)
  {
    returnVec[z] = location(zombies[z].x() + offset[zombies[z].facing()].x,zombies[z].y() + offset[zombies[z].facing()].y);
  }
  return returnVec;
}

vector<location> AI::zombieSightLocations()
{
  vector<location> returnVec;
   
  for(unsigned int z=0; z<zombies.size();z++)
  {
    //cout<<"Zombie at ("<<zombies[z].x()<<","<<zombies[z].y()<<") sees:"<<endl;
    int delta = zombies[z].sightRadius();
    for( int x=-delta;x<=delta;x++)
    {
      for( int y=-delta;y<=delta;y++)
      {
    //    cout<<"\tDistance: "<<distance(zombies[z].x(),zombies[z].y(),zombies[z].x() + x, zombies[z].y() + y)<<endl;
        if(distance(zombies[z].x(),zombies[z].y(),zombies[z].x() + x, zombies[z].y() + y) <= delta)
        {
          //cout<<"\t("<<zombies[z].x() + x<<","<<zombies[z].y() + y<<")"<<endl;
          returnVec.push_back(location(zombies[z].x() + x, zombies[z].y() + y));
        }      
      }
    }
  }
  
  return returnVec;
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

vector<location> getSafe(vector<location> open,vector<location> danger)
{
  vector<location> returnVec;
  //cout<<"In getSafe"<<endl;
  for(unsigned int o=0;o<open.size() ;o++)
  {
    //cout<<"Top open loop"<<endl;
    bool found = false;
    for(unsigned int d=0;d<danger.size()&& !found;d++)
    {
      if(open[o] == danger[d])
      {
        found = true;
      }
    }
    if(!found)
    {
      returnVec.push_back(open[o]);
    }
    else
    {
     // cout<<"Danger"<<endl;
    }
    found = false;
    //cout<<"Bottom Open Loop"<<endl;
  }
  return returnVec;
}

vector<location> getNoSight(vector<location> open, vector<location> danger)
{
  vector<location> returnVec;

  for(unsigned int o=0;o<open.size() ;o++)
  {
    bool found = false;
    for(unsigned int d=0;d<danger.size()&& !found;d++)
    {
      if(open[o] == danger[d])
      {
        found = true;
      }
    }
    if(!found)
    {
      returnVec.push_back(open[o]);
    }
    else
    {
     // cout<<"Danger"<<endl;
    }
    found = false;
  }
  return returnVec;
}

int getDirection(location me,location target)
{
  location delta = target - me;
  if(delta.x+delta.y > 0  )//to the right
  {
    if(delta.x>delta.y)
      return DOWN_RIGHT;
    else
      return UP_RIGHT;
  }
  else if(delta.x+delta.y<0)//to the left
  {
    if(delta.x>delta.y)
      return DOWN_LEFT;
    else
      return UP_LEFT;
  }
  else if(delta.x>0)
    return DOWN;
  else 
    return UP;
}


int turnTo(int myHeading, int goal)
{
  int delta = goal-myHeading;
  if(delta == 0) return 0;
//  cout<<"myHeading: "<<myHeading<<" Goal: "<<goal<<" Turn "<<((delta > 3 || delta < -3)?(delta>0?-1:1) :(delta>0?1:-1))<<endl;
  return ((delta > 3 || delta < -3)?(delta>0?-1:1) :(delta>0?1:-1)) ;
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
vector<Zombie> AI::zwithinRange(int x, int y,int range)
{
  vector<Zombie> inRange;
  for(unsigned int z=0;z<zombies.size();z++)
  {
    if(distance(x,y,zombies[z].x(),zombies[z].y())<= range)
    {
     // cout<<"Distance: "<<distance(x,y,zombies[z].x(),zombies[z].y())<<" ("<<x<<","<<y<<") to "<<"("<<zombies[z].x()<<","<<zombies[z].y()<<")"<<endl;
      inRange.push_back(zombies[z]);
    }
  }
  return inRange;
}


void displayWeapon(Weapon weapon)
{
  cout<<"\tName    :"<<weapon.name()<<" "<<weapon.id()<<endl
      <<"\tRange   :"<<weapon.range()<<endl
      <<"\tAmmo    :"<<weapon.ammo()<<endl
      <<"\tDelay   :"<<weapon.delay()<<endl
      <<"\tDamage  :"<<weapon.damage()<<endl
      <<"\tVSplashD:"<<weapon.verticalSplashDamage()<<endl
      <<"\tHSplashD:"<<weapon.horizontalSplashDamage()<<endl
      <<"\tHSplashR:"<<weapon.horizontalSplashRadius()<<endl;
}

location AI::meshWalls(int x, int y)
{
  bool noMesh=true;
  int index=-1,temp;
  int health = 999999;
  for(unsigned int i=0;i<6;i++)
  {
    location ret = location(x+offset[i].x,y+offset[i].y);
    if(ret.x%2==0 && ret.y%2==0 && atLocation(zombies,ret) == -1 && atLocation(humans,ret) == -1 && inBounds(ret.x,ret.y))
    {
      noMesh = false;
      if((temp = atLocation(walls,ret)) != -1)
      {
        if(walls[temp].hp() < health)
        {
          health = walls[temp].hp();
          index = temp;
        }
      
      }
      else
      {
        return ret;
      }
    }
  }
  if(noMesh)
  {
    return location(x,y);
  }
  return location(walls[index].x(),walls[index].y());
}

bool AI::runHuman()
{
  cout<<"Human Turn: "<<turnNum()<<endl;
  vector<location> zombieAttacks = zombieAttackLocations();
  zombieAttacks = zombieAttackLocations();
  //cout<<"After attackLocations"<<endl;
  vector<location> zombieSights = zombieSightLocations();
  
  if(turnNum() == 1)
  {
    for(unsigned int w=0;w<weapons.size();w++)
    {
      cout<<"WeaponID: "<<w<<endl;
      displayWeapon(weapons[w]);
    }
    vector<location> nearby(6);
    nearby[0] = location(1,0);
    nearby[1] = location(2,0);
    nearby[2] = location(3,0);
    nearby[3] = location(4,0);
    nearby[4] = location(5,0);
    nearby[5] = location(6,0);    
    for(int i=0;i<6;i++)
    {
      Human::spawn(weapons[0], nearby[i].x, nearby[i].y);
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
    if(humans[i].weaponID() != 0)
    {
      int index = watLocation(crates,humans[i].x(),humans[i].y());
      if(index != -1)
      {
        cout<<humans[i].id()<<"Grab: "<<crates[index].x()<<"."<<crates[index].y()<<") "<<endl;
        humans[i].grab(crates[index]);
      }
      else
      {
        cout<<humans[i].id()<<"NOTHING TO GRAB!! "<<humans[i].x()<<","<<humans[i].y()<<") "<<endl;
      }
    }
    if(humans[i].hp() - weapons[0].verticalSplashDamage()<= humans[i].maxHp() && weapons[humans[i].weaponID()].delay() < humans[i].idleTime() && 
       humans[i].ammo() > weapons[0].ammo())
    {
      cout<<"Healing "<<humans[i].id()<<" who has "<<humans[i].hp()<<"/"<<humans[i].maxHp()<<endl;
      humans[i].attack(humans[i].x(),humans[i].y());
    }
    //cout<<"Top of humans move"<<endl;
    bool blocked = false;
    while(humans[i].moves() && !blocked)//for(unsigned int m=0;m<humans[i].maxMoves();m++)
    {
      location myLoc = location(humans[i].x(),humans[i].y());
      if(weapons[humans[i].weaponID()].delay() < humans[i].idleTime())
      {
        vector<Zombie> inRange= zwithinRange(humans[i].x(),humans[i].y(),1);
        if(inRange.size() > 0)
        {
          int index = lowestHP(inRange);
          //int index = 0;
          if(index != -1)
          {
            humans[i].throwWeapon(humans[i].x(),humans[i].y());
            humans[i].attack(inRange[index]);
            cout<<humans[i].id()<<"CHAINSAWZS "<<inRange[index].x()<<","<<inRange[index].y()<<")("<<humans[i].x()<<","<<humans[i].y()<<endl;
            return false;
          }
        }
      }
      vector<location> nearby = validMoves(humans[i].x(),humans[i].y());
      int nearZ = nearest(zombies,humans[i].x(),humans[i].y());
      location head;
      if(nearZ != -1)
      {
        head = location((rand()%15)-7,(rand()%15)-7);
//        cout<<"heading: "<<head.x<<" "<<head.y<<endl;
      }
      else
      {
        head = location(0,0);
      }
      //cout<<"After get Nearby"<<endl;
      if(nearby.size() > 0)
      {
        //cout<<"Zombie attacks: "<<zombieAttacks.size()<<endl;
        //cout<<"Before Get safe"<<endl;
        vector<location> safe = getSafe(nearby,zombieAttacks);
        //cout<<"After get Safe"<<endl;
        if(safe.size()>0)
        {
          //cout<<"Before noSight"<<endl;
          vector<location> noSight = getNoSight(safe,zombieSights);
          //cout<<"After nosight"<<endl;
          if(noSight.size() >0)
          {
            int pick = nearestLoc(noSight,head.x,head.y);
            humans[i].move(noSight[pick].x,noSight[pick].y);
          }
          else//no nosight, but there are safe
          {
            int pick = nearestLoc(safe,head.x,head.y);
            humans[i].move(safe[pick].x,safe[pick].y);
          }
        }
        else//no safe, but there are nearby
        {
          int pick = nearestLoc(nearby,head.x,head.y);
          humans[i].move(nearby[pick].x,nearby[pick].y);
        }
        
      }
      else
      {
        cout<<"I am surrounded! Start Shooting! "<<endl;
        blocked = true;
      }
    }

  }
   cout<<"End Human Turn"<<endl;
  return true;
}

location AI::getEstimate()
{
  location estimate(0,0);
  int count=0;
  for(unsigned int i=0;i<zombies.size();i++)
  {
    if(zombies[i].smell() > 0)
    {
      count++;
      int dist = 6/zombies[i].smell();
      estimate += location(zombies[i].x() + offset[zombies[i].facing()].x*dist,zombies[i].y() + offset[zombies[i].facing()].y*dist);
    }
  }
  if(count>0)
  {
    return location(estimate.x/count,estimate.y/count);
  }
  return location(0,0);
}

int getBestSpawn(location estimate)
{
  if(abs(estimate.x)>abs(estimate.y))//0 or 2
  {
    if(estimate.x>0)
    {
      return 2;
    }
    return 0;
  }
  else
  {
    if(estimate.y>0)
    {
      return 1;
    }
    return 0;
  }
}

void AI::nearSpawn(int number,location estimate)
{
  int spawn = getBestSpawn(estimate);
  if(turnNum() == 2)
  {
    spawn = 0;
  }
  if(turnNum() == 4)
  {
    spawn = 1;
  }
  
  if(turnNum() == 6)
  {
    spawn = 3;
  }
  
  int direction=0;
  switch (spawn)
  {
    case 0:
      direction = DOWN_RIGHT;
      break;
    case 1:
      direction = DOWN_LEFT;
      break;
    case 2:
      direction = UP_LEFT;
      break;
    case 3:
      direction = UP_RIGHT;
      break;
  }
  for(int i=0;i<number;i++)
  {
    Zombie::spawn(spawnzones[spawn],direction);
  }
}

bool AI::runZombie()
{
  cout<<"----------------------------------Turn Number:"<<turnNum()<<endl;

  int diff = getZombieCap() - zombies.size();
  //gives you the amount of extra zombies you have;
  int extra = (getZombiesReady() - diff > 0)?getZombiesReady() - diff: 0;
  if(extra > 0)
  {  
    for(unsigned int i=0;i<zombies.size();i++)
    {
      if(zombies[i].hp() <70)
      {
        int index;
        //if you can eat infront of you  
        if( (index = atLocation(zombies, zombies[i].x()+offset[zombies[i].facing()].x,zombies[i].y()+offset[zombies[i].facing()].y)) != -1)
        {
          zombies[i].eat(zombies[index]);
          cout<<"EATING "<<turnNum()<<endl;
          return false;
        }
        if((index = atLocation(zombies,zombies[i].x(),zombies[i].y())) != -1 && index != i)
        {
          zombies[i].eat(zombies[index]);
          cout<<"EATING "<<turnNum()<<endl;
          return false;
        }
      }
    }
  }
  
  int freeToSpawn = (diff<getZombiesReady()?diff:getZombiesReady());
  location estimate = getEstimate();
  if(humans.size()>0)
    nearSpawn(freeToSpawn,location(humans[0].x(),humans[0].y()));
  else
    nearSpawn(freeToSpawn,estimate);

//end of my code  
  int index=0;
  bool actionMade = false;
  short filled[27][27];
  for(int i=0;i<27;i++)for(int j=0;j<27;j++)filled[i][j] =0;
  //See how many Zombies are in each square.
  for(unsigned int i = 0; i < zombies.size(); i++)
  {
	  filled[zombies[i].x()+13][zombies[i].y()+13]++;
  }
  
  
  for(unsigned int z=0;z<zombies.size();z++)
  {
    if(zombies[z].moves()>0)
    {
      DOMANSTAR(zombies[z],filled);
    }
  }
  return true;
}






void AI::DOMANSTAR(Zombie& THEMAN, short filling[27][27])
{
	//cout<<THEMAN.id()<<"  -  ENTERED DO MAN STAR"<<endl;
	int index;
	location myLoc(THEMAN.x(),THEMAN.y());
    location inFrontOfMe = myLoc + offset[THEMAN.facing()];
    short cream = 0;
	int turning = (rand()%2==0)?(TURN_LEFT):(TURN_RIGHT);

	if(inBounds(inFrontOfMe.x,inFrontOfMe.y))
    {
      cream = filling[13+inFrontOfMe.x][13+inFrontOfMe.y];
	}

	if(!inBounds(inFrontOfMe.x,inFrontOfMe.y))
    {
	  //cout<<THEMAN.id()<<"  Zombie is Turning: "<<THEMAN.x()<<","<<THEMAN.y()<<THEMAN.facing()<<endl;
      THEMAN.turn(TURN_LEFT);
    }
    //Find how filled I am
    //if there is a wall in front of you
    else if((index=atLocation(walls,inFrontOfMe))!=-1 && walls[index].hp() > 0)
    {
		//If there's a smell, attack.
		if(THEMAN.smell() > 0)
		{
		  //cout<<THEMAN.id()<<"  Zombie: "<<THEMAN.x()<<","<<THEMAN.y()<<THEMAN.facing()<<"  -  Wall: "<<walls[index].x()<<","<<walls[index].y()<<","<<walls[index].hp()<<endl;
          THEMAN.attack(walls[index]);
		}
		else
		{
		  int target = nearest(humans,myLoc);
          //if there is a human
          if(target != -1)
          {
            int heading = turnTo(THEMAN.facing(),getDirection(myLoc,location(humans[target].x(),humans[target].y())));
	        //And he's in front of you
            if(heading == 0)
            {
				cout<<THEMAN.id()<<"  Zombie: "<<THEMAN.x()<<","<<THEMAN.y()<<","<<THEMAN.facing()<<"  -  Wall: "<<walls[index].x()<<","<<walls[index].y()<<","<<walls[index].hp()<<endl;
           //Attack anyway
              THEMAN.attack(walls[index]);
            }
			else
			{
			  //cout<<THEMAN.id()<<"  Zombie is Turning: "<<THEMAN.x()<<","<<THEMAN.y()<<THEMAN.facing()<<endl;
              THEMAN.turn(heading);
			}
		  }
		  else
		  {
			  //There's NO smell, there's NO human, turn
			  //cout<<THEMAN.id()<<"  Zombie is Turning: "<<THEMAN.x()<<","<<THEMAN.y()<<THEMAN.facing()<<endl;
			  THEMAN.turn(turning);
		  }
		}
    }
    //if a human is directly in front of you
    else if((index=atLocation(humans,inFrontOfMe)) != -1)
    {
//      cout<<"I caught one mom!"<<endl;
	  cout<<THEMAN.id()<<"  Zombie: "<<THEMAN.x()<<","<<THEMAN.y()<<","<<THEMAN.facing()<<"  -  Human: "<<humans[index].x()<<","<<humans[index].y()<<","<<humans[index].hp()<<endl;
      THEMAN.attack(humans[index]);
    }
    else //if nothing is directly in front of you
    {
	  //Check for humans!
      int target = nearest(humans,myLoc);
      //if there is a human
      if(target != -1)
      {
        int heading = turnTo(THEMAN.facing(),getDirection(myLoc,location(humans[target].x(),humans[target].y())));
		//And he's in front of you
        if(heading == 0)
        {
		  //Move to him
		  //cout<<THEMAN.id()<<"  Zombie is Moving from: "<<THEMAN.x()<<","<<THEMAN.y()<<THEMAN.facing()<<endl;
		  if(cream < 11)
		  {
		    filling[myLoc.x+13][myLoc.y+13]--;
		    filling[inFrontOfMe.x+13][inFrontOfMe.y+13]++;
            THEMAN.move();
       }
        }
        else
        {   
			//If he's NOT in front of you.
            float stank = THEMAN.smell();
			int Hdist = distance(THEMAN.x(), THEMAN.y(), humans[target].x(), humans[target].y());
		    //If Dist to human is greater than 1/smell
			if(stank != 0 && Hdist >= (int) (1/stank))
			{
			  //Move toward smell.
			  //cout<<THEMAN.id()<<"  Zombie is Moving from: "<<THEMAN.x()<<","<<THEMAN.y()<<THEMAN.facing()<<endl;
			  if(cream < 11)
			  {
		    filling[myLoc.x+13][myLoc.y+13]--;
		    filling[inFrontOfMe.x+13][inFrontOfMe.y+13]++;
			    THEMAN.move();
			  }
			}
			else
			{
			  //Move toward human
			  //cout<<THEMAN.id()<<"  Zombie is Turning: "<<THEMAN.x()<<","<<THEMAN.y()<<THEMAN.facing()<<endl;
              THEMAN.turn(heading);
			}
        }
      }
      else
      {
	    //If your stank > 0, move to it
		//else, turn
	    float stank = THEMAN.smell();
        if(stank > 0)
        {
		  //cout<<THEMAN.id()<<"  Zombie is Moving from: "<<THEMAN.x()<<","<<THEMAN.y()<<THEMAN.facing()<<endl;
		  if(cream < 11)
			  {
		    filling[myLoc.x+13][myLoc.y+13]--;
		    filling[inFrontOfMe.x+13][inFrontOfMe.y+13]++;
            THEMAN.move();
         }
        }
        else
        {
		  //cout<<THEMAN.id()<<"  Zombie is Turning: "<<THEMAN.x()<<","<<THEMAN.y()<<THEMAN.facing()<<endl;
          THEMAN.turn(turning);
        }
      }
    }

}

