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

//Wall
class Wall
{
  _Wall* ptr;
 public:
  Wall();
  Wall(_Wall* ptr);
  int id();
  int x();
  int y();
  int hp();
  int maxHp();
  
  friend class Human;
  friend class Zombie;
};

//Crate
class Crate
{
  _Crate* ptr;
 public:
  Crate();
  Crate(_Crate* ptr);
  int id();
  int x();
  int y();
  int hp();
  int maxHp();
  int content();

  friend class Human;
};

//weapon
class Weapon
{
  _Weapon* ptr;
 public:
  Weapon();
  Weapon(_Weapon* ptr);
  int id();
  string name();
  int range();
  int ammo();
  int delay();
  int damage();
  int verticalSplashDamage();
  int horizontalSplashDamage();
  int horizontalSplashRadius();

  friend class Human;
};

//Human
class Human
{
  _Human* ptr;
 public:
  Human();
  Human(_Human* ptr);

  //Accessors
  int id();
  int x();
  int y();
  int hp();
  int maxHp();
  int moves();
  int maxMoves();
  int weaponID();
  int sightRadius();
  int ownerID();
  float idleTime();
  int ammo();
  int specialty();

  //Actions
  void move(int x, int y);
  void attack(const Zombie& target);
  void attack(const Wall& target);
  void attack(const Crate& target);
  void attack(int x, int y);
  void grab(const Crate& target);
  void throwWeapon(int x, int y);
  void throwAmmo(int x, int y, int quantity);
  void build(int x, int y);
  static void callWeaponDrop(const Weapon& weapon, int x, int y);
  static void callAmmoDrop(int x, int y);
  static void spawn(const Weapon& weapon, int x, int y);

  friend class Zombie;
};

//Zombie
class Zombie
{
  _Zombie* ptr;
 public:
  Zombie();
  Zombie(_Zombie* ptr);

  //Accessors
  int id();
  int x();
  int y();
  int hp();
  int maxHp();
  int moves();
  int maxMoves();
  int weaponID();
  int sightRadius();
  int ownerID();
  float idleTime();
  int facing();
  float smell();
  
  //Actions
  void move(int x, int y);
  void attack(const Human& target);
  void attack(const Wall& target);
  void attack(int x, int y);
  void eat(const Zombie& target);
  void turn(int direction);
  static void spawn(const Spawnzone& spawn, int facing);

  friend class Human;
};

//Airstrike
class Airstrike
{
  _Airstrike* ptr;
 public:
  Airstrike();
  Airstrike(_Airstrike* ptr);
  int id();
  int x();
  int y();
  int delay();
  int itemID();
};

//Spawnzone
class Spawnzone
{
  _Spawnzone* ptr;
 public:
  Spawnzone();
  Spawnzone(_Spawnzone* ptr);
  int id();
  int x1();
  int y1();
  int x2();
  int y2();

  friend class Zombie;
};

#endif
