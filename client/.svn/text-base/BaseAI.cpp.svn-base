//Copyright (C) 2009 - Missouri S&T ACM AI Team
//Please do not modify this file while building your AI
//See AI.h & AI.cpp for that

#include "BaseAI.h"
#include "game.h"

int BaseAI::turnNum()
{
  return getTurnNumber();
}

int BaseAI::mapSize()
{
  return getMapSize();
}

const char* BaseAI::humanTeamName()
{
  return getHumanTeamName();
}

const char* BaseAI::zombieTeamName()
{
  return getZombieTeamName();
}

float BaseAI::humanTime()
{
  return getHumanTime();
}

float BaseAI::zombieTime()
{
  return getZombieTime();
}

float BaseAI::airstrikesReady()
{
  return getAirstrikesReady();
}

int BaseAI::humansReady()
{
  return getHumansReady();
}

int BaseAI::zombiesReady()
{
  return getZombiesReady();
}

int BaseAI::zombieCap()
{
  return getZombieCap();
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

  if(turnNum() == 1)
  {
    init();
    if(!isHuman())
    {
      return true;
     }
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
