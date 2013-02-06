#include "AI.h"

const char* AI::username()
{
  return "ben";
}

const char* AI::password()
{
  return "ben";
}

bool AI::runHuman()
{
  if(turnNum() == 1)
  {
    spawnHumans();
  }
  
  return true;
}

void AI::spawnHumans()
{
  Human::spawn(weapons[0], 0, 0);
  Human::spawn(weapons[0], 1, 0);
  Human::spawn(weapons[0], 0, 1);
  Human::spawn(weapons[0], 1, 1);
  Human::spawn(weapons[0], -1, 0);
  Human::spawn(weapons[0], 0, -1);
}


bool AI::runZombie()
{
  if(zombies.size() < getZombieCap() && getZombiesReady() > 0)
    Zombie::spawn(spawnzones[rand() % spawnzones.size()], 0);
  
  for(int i=0; i < zombies.size(); ++i)
  {
    zombies[i].move( zombies[i].x()+1, zombies[i].y() );
  }

  return true;
} 
