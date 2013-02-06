#include "BaseAI.h"

int BaseAI::turnNum()
{
  return getTurnNumber();
}

bool BaseAI::startTurn()
{
  int count = 0;
  count = getWallCount();
  walls.clear();
  walls.resize(count);
  for(int i = 0; i < count; i++)
  {
    walls[i] = Wall(getWalls()+i);
  }
  count = getCrateCount();
  crates.clear();
  crates.resize(count);
  for(int i = 0; i < count; i++)
  {
    crates[i] = Crate(getCrates()+i);
  }
  count = getWeaponCount();
  weapons.clear();
  weapons.resize(count);
  for(int i = 0; i < count; i++)
  {
    weapons[i] = Weapon(getWeapons()+i);
  }
  count = getHumanCount();
  humans.clear();
  humans.resize(count);
  for(int i = 0; i < count; i++)
  {
    humans[i] = Human(getHumans()+i);
  }
  count = getZombieCount();
  zombies.clear();
  zombies.resize(count);
  for(int i = 0; i < count; i++)
  {
    zombies[i] = Zombie(getZombies()+i);
  }
  count = getAirstrikeCount();
  airstrikes.clear();
  airstrikes.resize(count);
  for(int i = 0; i < count; i++)
  {
    airstrikes[i] = Airstrike(getAirstrikes()+i);
  }
  count = getSpawnzoneCount();
  spawnzones.clear();
  spawnzones.resize(count);
  for(int i = 0; i < count; i++)
  {
    spawnzones[i] = Spawnzone(getSpawnzones()+i);
  }
  
  if(isHuman())
  {
    return runHuman();
  }
  else
  {
    return runZombie();
  }
}
