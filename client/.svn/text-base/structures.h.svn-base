//Copyright (C) 2009 - Missouri S&T ACM AI Team
//Please do not modify this file while building your AI
//See AI.h & AI.cpp for that
#ifndef STRUCTURES_H
#define STRUCTURES_H

struct _Wall
{
  int id, x, y, hp, maxhp;
};

struct _Crate
{
  int id, x, y, hp, maxhp, content;
};

struct _Weapon
{
  int id;
  char* name;
  int range, ammo, damage, vrtSplashDam, horSplashDam, horSplashRad;
  float delay;
  bool canSpawnWith, canCall;
};

//("Human" id x y curHealth overHeal moves maxMoves weaponID sight ownerID idletime ammo specialty)
struct _Human
{
  int id, x, y, hp, maxhp, moves, maxmoves, weaponid, sight, ownerid;
  float idletime;
  int ammo, specialty;
};

//("Zombie" id x y curHealth overHeal moves maxMoves weaponID sight ownerID idletime facing smell)
struct _Zombie
{
  int id, x, y, hp, maxhp, moves, maxmoves, weaponid, sight, ownerid;
  float idletime;
  int facing;
  float smell;
};

//("Airstrike" id x y delay itemID)
struct _Airstrike
{
  int id, x, y, delay, itemid;
};

//("Spawn" id x1 y1 x2 y2)
struct _Spawnzone
{
  int id, x1, y1, x2, y2;
};

#endif
