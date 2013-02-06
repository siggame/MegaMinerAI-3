#ifndef BASEAI_H
#define BASEAI_H

#include <vector>
#include "wrappers.h"
#include "game.h"

class BaseAI
{
protected:
  std::vector<Wall> walls;
  std::vector<Crate> crates;
  std::vector<Weapon> weapons;
  std::vector<Human> humans;
  std::vector<Zombie> zombies;
  std::vector<Airstrike> airstrikes;
  std::vector<Spawnzone> spawnzones;
public:
  virtual const char* username() = 0;
  virtual const char* password() = 0;
  virtual bool runHuman() = 0;
  virtual bool runZombie() = 0;
  bool startTurn();
  int turnNum();
};

#endif
