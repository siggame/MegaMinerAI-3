//Copyright (C) 2009 - Missouri S&T ACM AI Team
//Please do not modify this file while building your AI
//See AI.h & AI.cpp for that
#ifndef BASEAI_H
#define BASEAI_H

#include <vector>
#include <ctime>
#include "wrappers.h"
#include "game.h"

#define DOWN 5
#define DOWN_LEFT 4
#define UP_LEFT 3
#define UP 2
#define UP_RIGHT 1
#define DOWN_RIGHT 0


#define TURN_LEFT 1
#define TURN_RIGHT -1

/// \brief A basic AI interface.

///This class implements most the code an AI would need to interface with the lower-level game code.
///AIs should extend this class to get a lot of builer-plate code out of the way
///The provided AI class does just that.
class BaseAI
{
protected:
  std::vector<Wall> walls;              ///< All walls that are visible during the current turn
  std::vector<Crate> crates;            ///< All crates that are visible during the current turn
  std::vector<Weapon> weapons;          ///< All weapons in the game
  std::vector<Human> humans;            ///< All humans that are visible during the current turn
  std::vector<Zombie> zombies;          ///< All zombies that are visible during the current turn
  std::vector<Airstrike> airstrikes;    ///< All airstrikes that exist during the current turn
  std::vector<Spawnzone> spawnzones;    ///< All spawnzones in the game
public:
  ///
  ///Make this your username, which should be provided.
  virtual const char* username() = 0;
  ///
  ///Make this your password, which should be provided.
  virtual const char* password() = 0;
  ///
  ///This is run on turn 1 before runHuman or runZombie
  virtual void init() = 0;
  ///
  ///This is run every turn when the AI is playing a human. Return true to end the turn, return false
  ///to request a status update from the server and then immediately rerun this function with the
  ///latest game status.
  virtual bool runHuman() = 0;
  ///
  ///This is run every turn when the AI is playing a zombie. Return true to end the turn, return false
  ///to request a status update from the server and then immediately rerun this function with the
  ///latest game status.
  virtual bool runZombie() = 0;

  bool startTurn();

  ///Current turn number.
  int turnNum();
  ///Size of the map.
  int mapSize();
  ///Name of the human player.
  const char* humanTeamName();
  ///Name of the zombie player.
  const char* zombieTeamName();
  ///Time in seconds human player has remaining.
  float humanTime();
  ///Time in seconds zombie player has remaining.
  float zombieTime();
  ///Number of airstrikes ready to be launched
  float airstrikesReady();
  ///Zombies that can spawn (may exceed cap)
  int zombiesReady();
  ///Humans that can spawn
  int humansReady();
  ///Number of zombies that may live at a time
  int zombieCap();
};

#endif
