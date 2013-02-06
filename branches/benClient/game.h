#ifndef GAME_H
#define GAME_H

#include "network.h"
#include "structures.h"

extern "C"
{
  void login(int socket, const char* username, const char* password);
  void createGame();
  void joinGame(int id);
  
  void endTurn();
  void getStatus();
  
  
//commands
  bool humanMove(_Human* unit, int x, int y);
  bool humanAttack(_Human* unit, _Zombie* target);
  bool humanAttackCrate(_Human* unit, _Crate* target);
  bool humanAttackWall(_Human* unit, _Wall* target);
  bool humanAttackGround(_Human* unit, int x, int y);
  bool humanGrab(_Human* unit, _Crate* target);
  bool humanThrowWeapon(_Human* unit, int x, int y);
  bool humanThrowAmmo(_Human* unit, int x, int y, int quantity);
  bool humanBuild(_Human* unit, int x, int y);
  bool humanCallWeapon(_Weapon* weapon, int x, int y);
  bool humanCallAmmo(int x, int y);
  bool humanSpawn(_Weapon* weapon, int x, int y);
  
  bool zombieMove(_Zombie* unit, int x, int y);
  bool zombieAttack(_Zombie* unit, _Human* target);
  bool zombieAttackWall(_Zombie* unit, _Wall* target);
  bool zombieAttackGround(_Zombie* unit, int x, int y);
  bool zombieEat(_Zombie* unit, _Zombie* target);
  bool zombieTurn(_Zombie* unit, int direction);
  bool zombieSpawn(_Spawnzone* spawn, int facing);
  
//accessors
  int getTurnNumber();
  float getAirstrikesReady();
  int getZombiesReady();
  int getHumansReady();
  int getZombieCap();

  _Wall* getWalls();
  int getWallCount();

  _Crate* getCrates();
  int getCrateCount();

  _Weapon* getWeapons();
  int getWeaponCount();

  _Human* getHumans();
  int getHumanCount();

  _Zombie* getZombies();
  int getZombieCount();

  _Airstrike* getAirstrikes();
  int getAirstrikeCount();

  _Spawnzone* getSpawnzones();
  int getSpawnzoneCount();
  bool isHuman();
  
  
  int networkLoop(int socket);
}


#endif
