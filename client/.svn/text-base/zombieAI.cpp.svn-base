#include "AI.h"

bool AI::runZombie()
{
  Zombie::spawn(spawnzones[0],0);
  for(unsigned int i=0;i<zombies.size();i++)
  {
    short move;
    move = rand()%3;
	if(move == 0)
	  zombies[i].move();
	if(move == 1)
	  zombies[i].turn(-1);
	if(move == 2)
	  zombies[i].turn(1);
  }

  return true;
}
