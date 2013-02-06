#include "AI.h"

vector<int> MANSTAR;

const char* AI::username()
{
  return "LUMBERJACKS";
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

int turnTo(int myHeading, int goal)
{
  int delta = goal-myHeading;
  if(delta == 0) return 0;
//  cout<<"myHeading: "<<myHeading<<" Goal: "<<goal<<" Turn "<<((delta > 3 || delta < -3)?(delta>0?-1:1) :(delta>0?1:-1))<<endl;
  return ((delta > 3 || delta < -3)?(delta>0?-1:1) :(delta>0?1:-1)) ;
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

//Checks if I should MANSTAR
bool MANCHECK(int checkval)
{
	bool ret = false;

	for(unsigned int i = 0; i < MANSTAR.size(); i++)
	{
		if(MANSTAR[i] == checkval)
		{
			ret = true;
		}
	}

	return ret;
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
		  if(cream < 6)
            THEMAN.move();
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
			  if(cream < 6)
			    THEMAN.move();
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
		  if(cream < 6)
            THEMAN.move();
        }
        else
        {
		  //cout<<THEMAN.id()<<"  Zombie is Turning: "<<THEMAN.x()<<","<<THEMAN.y()<<THEMAN.facing()<<endl;
          THEMAN.turn(turning);
        }
      }
    }

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
  short filled[27][27];
  //Initialize Filled Just In Case
  for(unsigned int i = 0; i < 27; i++)
  {
	  for(unsigned int j=0; j <27; j++)
	  {
		  filled[i][j] = 0;
	  }
  }
  //See how many Zombies are in each square.
  for(unsigned int i = 0; i < zombies.size(); i++)
  {
	  filled[zombies[i].x()+13][zombies[i].y()+13]++;
  }

  cout<<"----------------------------------Turn Number:"<<turnNum()<<endl;
  for(unsigned int i = 0; i < humans.size(); i++)
  {
	  cout<<"Human ID "<<humans[i].id()<<"   -   Location: "<<humans[i].x()<<","<<humans[i].y()<<"   -   Health: "<<humans[i].hp()<<endl;
  }
  int freeToSpawn = (getZombieCap() - zombies.size()<getZombiesReady()?getZombieCap() - zombies.size():getZombiesReady());
  
  for(int s=0;s<freeToSpawn;s++)
  {
		Zombie::spawn(spawnzones[1],DOWN_LEFT);
  }
  int index=0;
  
    for(unsigned int z=0;z<zombies.size();z++)
    {
	    //If you're on the dumb areas
	  if(zombies[z].x() == 9 ||zombies[z].x() == -9)
      {
	  	  //Are you facing the wrong way?
	     if(zombies[z].facing() == 4)
		  {
		    //cout<<zombies[z].id()<<"  Zombie is Turning 1: "<<zombies[z].x()<<","<<zombies[z].y()<<","<<zombies[z].facing()<<endl;
            zombies[z].turn(TURN_RIGHT);
		  }
		  else
		  {
		    //cout<<zombies[z].id()<<"  Zombie is Moving From 1: "<<zombies[z].x()<<","<<zombies[z].y()<<","<<zombies[z].facing()<<endl;
            zombies[z].move();
		  }
	  }
	}

  
  if(turnNum() > 50)
  {
	  //Begin Clearcutting.
	  int farthest = 13;
	  bool advance = true;
	  //Find the zombie with the LOWEST Y.
      for(unsigned int z=0;z<zombies.size();z++)
      {
         if(zombies[z].y() < farthest)
		   farthest = zombies[z].y();
	  }
	  //For ALL ZOMBIES at this X, see if we can ALL move forward.
	  for(unsigned int z=0;z<zombies.size();z++)
      {
         if(zombies[z].y() == farthest && abs(zombies[z].x()) != 8)
		 {
             location myLoc(zombies[z].x(),zombies[z].y());
             location inFrontOfMe = myLoc + offset[zombies[z].facing()];
			 //Is a wall blocking?
             if((index=atLocation(walls,inFrontOfMe))!=-1 && walls[index].hp() > 10)
             {
				 advance = false;
			 }
			 //Is a human blocking?
             if((index=atLocation(humans,inFrontOfMe)) != -1)
             {
				 advance = false;
			 }
		 }
	  }
			 


  for(unsigned int z=0;z<zombies.size();z++)
  {

	if(MANCHECK(zombies[z].id()))
	{
		DOMANSTAR(zombies[z], filled);
		///DO MANSTAR
	}
	else
	{
      location myLoc(zombies[z].x(),zombies[z].y());
      location inFrontOfMe = myLoc + offset[zombies[z].facing()];


		//DO FORMATION
      if((index=atLocation(walls,inFrontOfMe))!=-1 && walls[index].hp() > 0)
      {
		 //cout<<zombies[z].id()<<"  Zombie: "<<zombies[z].x()<<","<<zombies[z].y()<<","<<zombies[z].facing()<<"  -  Wall: "<<walls[index].x()<<","<<walls[index].y()<<","<<walls[index].hp()<<endl;
         zombies[z].attack(walls[index]);
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
	    //If I'm at bounds, turn around.
        if(zombies[z].y() < -12)
	    {
	  	    MANSTAR.push_back(zombies[z].id());
			//cout<<zombies[z].id()<<"  Zombie is Turning 2: "<<zombies[z].x()<<","<<zombies[z].y()<<","<<zombies[z].facing()<<endl;
		    zombies[z].turn(TURN_LEFT);
	    }
		else if(zombies[z].y() > 12 && zombies[z].facing() != 4)
		{
			//Am I facing the right way?
			//cout<<zombies[z].id()<<"  Zombie is Turning 3: "<<zombies[z].x()<<","<<zombies[z].y()<<","<<zombies[z].facing()<<endl;
			zombies[z].turn(TURN_LEFT);
		}
	    else
	    {
		  //Am I farthest?
	  	  if(zombies[z].y() == farthest)
		  {
			  //Can we advance?
			  if(advance)
			  {
			  //cout<<zombies[z].id()<<"  Zombie is Moving From 2: "<<zombies[z].x()<<","<<zombies[z].y()<<","<<zombies[z].facing()<<endl;
              zombies[z].move();
			  }
		  }
		  //Am I not the furthest?
		  else
		  {
			  //cout<<zombies[z].id()<<"  Zombie is Moving From 3: "<<zombies[z].x()<<","<<zombies[z].y()<<","<<zombies[z].facing()<<endl;
              zombies[z].move();
		  }
	    }
      }
	}
  }
  //Outside of For Loop
  }
  cout<<"TURN ENDING LOL---------------------------------------"<<endl;
  return true;
}
