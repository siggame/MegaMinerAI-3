//Copyright (C) 2009 - Missouri S&T ACM AI Team
//Please do not modify this file while building your AI
//See AI.h & AI.cpp for that
#ifndef _WRAPPERS_H
#define _WRAPPERS_H

#include "structures.h"

#include <string>
using std::string;

//Predeclare these classes so they can rely on each other
class Zombie;
class Human;
class Crate;
class Spawnzone;
class Airstrike;

///Wall
class Wall
{
  _Wall* ptr;
 public:
  Wall();
  Wall(_Wall* ptr);
  ///Unique ID number
  int id();
  ///X coordinate
  int x();
  ///Y coordinate
  int y();
  ///Current HP
  int hp();
  ///Maximum HP
  int maxHp();

  friend class Human;
  friend class Zombie;
};

///Crate
class Crate
{
  _Crate* ptr;
 public:
  Crate();
  Crate(_Crate* ptr);
  ///Unique ID number
  int id();
  ///X coordinate
  int x();
  ///Y coordinate
  int y();
  ///Current HP
  int hp();
  ///Maximum HP
  int maxHp();
  ///Contents of the crate; Weapon ID or -1 for ammo
  int content();

  friend class Human;
};


///Weapon
class Weapon
{
  _Weapon* ptr;
 public:
  Weapon();
  Weapon(_Weapon* ptr);
  ///Unique ID
  int id();
  ///The weapon's name
  string name();
  ///The number of hexes a target must be within to be attacked
  int range();
  ///The amount of ammo each attack requires with the weapon
  int ammo();
  ///The number of unitts of idleTime firing uses
  float delay();
  ///The damage dealt on an attack
  int damage();
  ///Damage dealt to all units other than the target in the hex
  int verticalSplashDamage();
  ///Damage dealt to all units in the horizontal splash radius
  int horizontalSplashDamage();
  ///Number of hexes from the center horizontal splash spreads
  int horizontalSplashRadius();
  ///True if humans can start with this weapon
  bool canSpawnWith();
  ///True if humans can call for an airdrop with this weapon
  bool canCall();

  friend class Human;
};

///Human
class Human
{
  _Human* ptr;
 public:
  Human();
  Human(_Human* ptr);

  //Accessors
  ///Unique ID
  int id();
  ///X coordinate
  int x();
  ///Y coordinate
  int y();
  ///Current HP
  int hp();
  ///Maximum HP
  int maxHp();
  ///Moves left this turn
  int moves();
  ///Maximum moves per turn
  int maxMoves();
  ///Weapon ID
  int weaponID();
  ///Number of hexes the unit can see
  int sightRadius();
  ///ID of the owning player.
  int ownerID();
  ///Amount of time saved up for using weapons.
  float idleTime();
  ///Ammunition on hand
  int ammo();
  ///Weapon specialty
  int specialty();

  //Actions
  ///Move to the adjacent hex at x y
  void move(int x, int y);
  ///Attack the Zombie
  void attack(const Zombie& target);
  ///Attack the Wall
  void attack(const Wall& target);
  ///Attack the Crate. Crate may explode
  void attack(const Crate& target);
  ///Attack hex at X,Y
  void attack(int x, int y);
  ///Take the contents of a crate in the same hex
  void grab(const Crate& target);
  ///Give ammo to an adjacent friend
  void give(const Human& target, int quantity);
  ///Drops equipped weapon in an adjacent hex
  void throwWeapon(int x, int y);
  ///Drops an amount of ammo in an adjacent hex
  void throwAmmo(int x, int y, int quantity);

  ///Build or reenforce a wall in an adjacent square
  void build(int x, int y);
  ///Orders a crate with a weapon produced at x,y
  static void callWeaponDrop(const Weapon& weapon, int x, int y);
  ///Orders a crate of ammo at x,y
  static void callAmmoDrop(int x, int y);
  ///Creates a human at x,y wielding the chosen weapon
  static void spawn(const Weapon& weapon, int x, int y);

  friend class Zombie;
};

///Zombie
class Zombie
{
  _Zombie* ptr;
 public:
  Zombie();
  Zombie(_Zombie* ptr);

  //Accessors
  ///Unique ID
  int id();
  ///X coordinate
  int x();
  ///Y coordinate
  int y();
  ///Current HP
  int hp();
  ///Maximum HP
  int maxHp();
  ///Moves left this turn
  int moves();
  ///Maximum moves per turn
  int maxMoves();
  ///Weapon ID
  int weaponID();
  ///Number of hexes the unit can see
  int sightRadius();
  ///ID of the owning player.
  int ownerID();
  ///Amount of time saved up for using weapons.
  float idleTime();
  ///Direction the Zombie is can attack or move in
  int facing();
  ///Scent, see gamemanual.txt
  float smell();

  //Actions
  ///Move forward
  void move();
  ///Attack the human
  void attack(const Human& target);
  ///Attack the wall
  void attack(const Wall& target);
  ///Eat a buddy, healing self 200% of the zombie's hp
  void eat(const Zombie& target);
  ///Turn left (1) or right (-1)
  void turn(int direction);
  ///Create a zombie in the spawn point
  static void spawn(const Spawnzone& spawn, int facing);

  friend class Human;
};

///Airstrike
class Airstrike
{
  _Airstrike* ptr;
 public:
  Airstrike();
  Airstrike(_Airstrike* ptr);
  ///Unique ID
  int id();
  ///X location
  int x();
  ///Y location
  int y();
  ///Turns until landing
  int delay();
  ///Weapon being dropped (ammo == -1)
  int itemID();
};

///Spawnzone
class Spawnzone
{
  _Spawnzone* ptr;
 public:
  Spawnzone();
  Spawnzone(_Spawnzone* ptr);
  ///Unique ID
  int id();
  ///line start x coordinate
  int x1();
  ///line start y coordinate
  int y1();
  ///line end x coordinate
  int x2();
  ///line end y coordinate
  int y2();

  friend class Zombie;
};

#endif
