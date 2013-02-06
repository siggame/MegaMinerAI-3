#include "AI.h"
const char* AI::username()
{
  return "Ghost AI";
}

const char* AI::password()
{
  return "ROBOTHOUSE";
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

vector<int> AI::getAdjacentWalls(location myLoc)
{
  vector<location> adjacent = getAdjacent(myLoc);
  vector<int> returnVec;
  int temp=-1;
  for(unsigned int a=0;a<adjacent.size();a++)
  {
    temp=atLocation(walls,adjacent[a].x,adjacent[a].y);
    if( temp != -1 )
    {
      returnVec.push_back(temp);
    }
  }
  return returnVec;
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

vector<location> AI::validMoves(location myLoc)
{
  return validMoves(myLoc.x,myLoc.y);
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
    if(distance(x,y,zombies[z].x(),zombies[z].y())<= range)
    {
      inRange.push_back(location(zombies[z].x(),zombies[z].y()));
    }
  }
  return inRange;
}

void AI::displayZombie(Zombie zombie)
{
  cout<<"\tHP:\t"<<zombie.hp()<<"/"<<zombie.maxHp()<<endl
      <<"\tWeapon:\t#"<<zombie.weaponID()<<" "<<weapons[zombie.weaponID()].name()<<endl
      <<"\tSight:\t"<<zombie.sightRadius()<<endl
      <<"\tSmell:\t"<<zombie.smell()<<endl
      <<"\tIdle:\t"<<zombie.idleTime()<<endl;
}

void AI::displayHuman(Human human)
{
  cout<<"\tHP:\t"<<human.hp()<<"/"<<human.maxHp()<<endl
      <<"\tWeapon:\t#"<<human.weaponID()<<" "<<weapons[human.weaponID()].name()<<endl
      <<"\tWeapon Special:\t#"<<human.specialty()<<" "<<weapons[human.specialty()].name()<<endl
      <<"\tAmmo:\t"<<human.ammo()<<endl
      <<"\tIdle:\t"<<human.idleTime()<<endl;
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
    vector<location> nearby(6);
    nearby[0] = location(2 ,5);
    nearby[1] = location(1 ,5);
    nearby[2] = location(0 ,5);
    nearby[3] = location(-1,5);
    nearby[4] = location(-2,5);
    nearby[5] = location(-3,5);    
    for(int i=0;i<getHumansReady();i++)
    {
      Human::spawn(weapons[2], nearby[i].x, nearby[i].y);
    }
  }
  if(turnNum() == 3)
  {
    for(unsigned int i=0;i<humans.size();i++)    
    {
      cout<<"Human["<<humans[i].id()<<"]"<<endl;
      displayHuman(humans[i]);
    }
  }
  
  for(unsigned int h=0;h<humans.size();h++)
  {
    //humans[h].attack(humans[h].x(),humans[h].y());
    location myLoc(humans[h].x(),humans[h].y());
    vector<location> nearby = validMoves(myLoc);
    if(nearby.size() >0)
    {
      humans[h].build(nearby[0].x,nearby[0].y);
    }
    else
    {
      int target = lowestHealth(walls,getAdjacentWalls(myLoc));
      if(target != -1)
      {
//        cout<<"Healing wall with: "<<walls[target].hp()<<endl;
        humans[h].build(walls[target].x(),walls[target].y());
      }
    }
  }
  return true;
}

bool AI::runZombie()
{
  cout<<"----------------------------------Turn Number:"<<turnNum()<<endl;
  for(unsigned int i = 0; i < humans.size(); i++)
  {
	  cout<<"Human ID "<<humans[i].id()<<"   -   Location: "<<humans[i].x()<<","<<humans[i].y()<<"   -   Health: "<<humans[i].hp()<<endl;
  }
  int freeToSpawn = (getZombieCap() - zombies.size()<getZombiesReady()?getZombieCap() - zombies.size():getZombiesReady());
  for(int s=0;s<freeToSpawn;s++)
  {
    int spot = rand()%2;
	if(spot == 0)
	{
		Zombie::spawn(spawnzones[0],DOWN_LEFT);
	}
	else
	{
        Zombie::spawn(spawnzones[1],UP_LEFT);
	}
  }
  int index=0;
  
  for(unsigned int z=0;z<zombies.size();z++)
  {
    location myLoc(zombies[z].x(),zombies[z].y());
    location inFrontOfMe = myLoc + offset[zombies[z].facing()];
    //if you are about to run out of bounds
    if(!inBounds(inFrontOfMe.x,inFrontOfMe.y))
    {
      zombies[z].turn(TURN_LEFT);
    }
    //if there is a wall in front of you
    else if((index=atLocation(walls,inFrontOfMe))!=-1 && walls[index].hp() > 0)
    {
		//If there's a smell, attack.
		if(zombies[z].smell() > 0)
		{
		  	//cout<<zombies[z].id()<<"  Zombie: "<<zombies[z].x()<<","<<zombies[z].y()<<zombies[z].facing()<<"  -  Wall: "<<walls[index].x()<<","<<walls[index].y()<<","<<walls[index].hp()<<endl;
          zombies[z].attack(walls[index]);
		}
		else
		{
		  int target = nearest(humans,myLoc);
          //if there is a human
          if(target != -1)
          {
            int heading = turnTo(zombies[z].facing(),getDirection(myLoc,location(humans[target].x(),humans[target].y())));
	        //And he's in front of you
            if(heading == 0)
            {
				//cout<<zombies[z].id()<<"  Zombie: "<<zombies[z].x()<<","<<zombies[z].y()<<","<<zombies[z].facing()<<"  -  Wall: "<<walls[index].x()<<","<<walls[index].y()<<","<<walls[index].hp()<<endl;
           //Attack anyway
              zombies[z].attack(walls[index]);
            }
			else
			{
              zombies[z].turn(heading);
			}
		  }
		  else
		  {
			  //There's NO smell, there's NO human, turn
		      zombies[z].turn(TURN_LEFT);
		  }
		}
    }
    //if a human is directly in front of you
    else if((index=atLocation(humans,inFrontOfMe)) != -1)
    {
//      cout<<"I caught one mom!"<<endl;
	  cout<<zombies[z].id()<<"  Zombie: "<<zombies[z].x()<<","<<zombies[z].y()<<","<<zombies[z].facing()<<"  -  Human: "<<humans[index].x()<<","<<humans[index].y()<<","<<humans[index].hp()<<endl;
      zombies[z].attack(humans[index]);
    }
    else //if nothing is directly in front of you
    {
	  //Check for humans!
      int target = nearest(humans,myLoc);
      //if there is a human
      if(target != -1)
      {
        int heading = turnTo(zombies[z].facing(),getDirection(myLoc,location(humans[target].x(),humans[target].y())));
		//And he's in front of you
        if(heading == 0)
        {
		  //Move to him
          zombies[z].move();
        }
        else
        {   
			//If he's NOT in front of you.
            float stank = zombies[z].smell();
			int Hdist = distance(zombies[z].x(), zombies[z].y(), humans[target].x(), humans[target].y());
		    //If Dist to human is greater than 1/smell
			if(stank != 0 && Hdist >= (int) (1/stank))
			{
			  //Move toward smell.
			  zombies[z].move();
			}
			else
			{
			  //Move toward human
              zombies[z].turn(heading);
			}
        }
      }
      else
      {
	    //If your stank > 0, move to it
		//else, turn
	    float stank = zombies[z].smell();
        if(stank > 0)
        {
          zombies[z].move();
        }
        else
        {
          zombies[z].turn(TURN_LEFT);
        }
      }
    }
  }
  return true;
}
