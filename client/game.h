//Copyright (C) 2009 - Missouri S&T ACM AI Team
//Please do not modify this file while building your AI
//See AI.h & AI.cpp for that
#ifndef GAME_H
#define GAME_H

#include "network.h"
#include "structures.h"

#ifdef WIN32
#define DLLEXPORT extern "C" __declspec(dllexport)
#else
#define DLLEXPORT
#endif

#define UP 2
#define UP_RIGHT 1
#define DOWN_RIGHT 0
#define DOWN 5
#define DOWN_LEFT 4
#define UP_LEFT 3

#define TURN_LEFT 1
#define TURN_RIGHT -1

extern "C"
{
  extern char** _argv;
  extern int    _argc;

  DLLEXPORT bool login(int socket, const char* username, const char* password);
  DLLEXPORT void createGame();
  DLLEXPORT void joinGame(int id);

  DLLEXPORT void endTurn();
  DLLEXPORT void getStatus();


//commands
  DLLEXPORT bool humanMove(_Human* unit, int x, int y);
  DLLEXPORT bool humanAttack(_Human* unit, _Zombie* target);
  DLLEXPORT bool humanAttackCrate(_Human* unit, _Crate* target);
  DLLEXPORT bool humanAttackWall(_Human* unit, _Wall* target);
  DLLEXPORT bool humanAttackGround(_Human* unit, int x, int y);
  DLLEXPORT bool humanGrab(_Human* unit, _Crate* target);
  DLLEXPORT bool humanGive(_Human* unit, _Human* target, int quantity);
  DLLEXPORT bool humanThrowWeapon(_Human* unit, int x, int y);
  DLLEXPORT bool humanThrowAmmo(_Human* unit, int x, int y, int quantity);
  DLLEXPORT bool humanBuild(_Human* unit, int x, int y);
  DLLEXPORT bool humanCallWeapon(_Weapon* weapon, int x, int y);
  DLLEXPORT bool humanCallAmmo(int x, int y);
  DLLEXPORT bool humanSpawn(_Weapon* weapon, int x, int y);

  DLLEXPORT bool zombieMove(_Zombie* unit);
  DLLEXPORT bool zombieAttack(_Zombie* unit, _Human* target);
  DLLEXPORT bool zombieAttackWall(_Zombie* unit, _Wall* target);
  DLLEXPORT bool zombieAttackGround(_Zombie* unit, int x, int y);
  DLLEXPORT bool zombieEat(_Zombie* unit, _Zombie* target);
  DLLEXPORT bool zombieTurn(_Zombie* unit, int direction);
  DLLEXPORT bool zombieSpawn(_Spawnzone* spawn, int facing);

//accessors
  DLLEXPORT int getTurnNumber();
  DLLEXPORT float getAirstrikesReady();
  DLLEXPORT int getZombiesReady();
  DLLEXPORT int getHumansReady();
  DLLEXPORT int getZombieCap();
  DLLEXPORT char* getHumanTeamName();
  DLLEXPORT char* getZombieTeamName();
  DLLEXPORT float getHumanTime();
  DLLEXPORT float getZombieTime();
  DLLEXPORT int getMapSize();

  DLLEXPORT _Wall* getWalls(int num = 0);
  DLLEXPORT int getWallCount();

  DLLEXPORT _Crate* getCrates(int num = 0);
  DLLEXPORT int getCrateCount();

  DLLEXPORT _Weapon* getWeapons(int num = 0);
  DLLEXPORT int getWeaponCount();

  DLLEXPORT _Human* getHumans(int num = 0);
  DLLEXPORT int getHumanCount();

  DLLEXPORT _Zombie* getZombies(int num = 0);
  DLLEXPORT int getZombieCount();

  DLLEXPORT _Airstrike* getAirstrikes(int num = 0);
  DLLEXPORT int getAirstrikeCount();

  DLLEXPORT _Spawnzone* getSpawnzones(int num = 0);
  DLLEXPORT int getSpawnzoneCount();
  DLLEXPORT bool isHuman();


  DLLEXPORT int networkLoop(int socket, bool practice = false);
}


#endif
