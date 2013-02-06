#include "wrappers.h"
#include "game.h"

//Wall
Wall::Wall()
{

}

Wall::Wall(_Wall* ptr) : ptr(ptr)
{

}

int Wall::id()
{
  return ptr->id;
}


int Wall::x()
{
  return ptr->x;
}

int Wall::y()
{
  return ptr->y;
}

int Wall::hp()
{
  return ptr->hp;
}

int Wall::maxHp()
{
  return ptr->maxhp;
}


//Crate
Crate::Crate()
{

}

Crate::Crate(_Crate* ptr) : ptr(ptr)
{

}

int Crate::id()
{
  return ptr->id;
}

int Crate::x()
{
  return ptr->x;
}

int Crate::y()
{
  return ptr->y;
}

int Crate::hp()
{
  return ptr->hp;
}

int Crate::maxHp()
{
  return ptr->maxhp;
}

int Crate::content()
{
  return ptr->content;
}

//weapon

Weapon::Weapon()
{

}

Weapon::Weapon(_Weapon* ptr) : ptr(ptr)
{

}

int Weapon::id()
{
  return ptr->id;
}

string Weapon::name()
{
  return string(ptr->name);
}

int Weapon::range()
{
  return ptr->range;
}

int Weapon::ammo()
{
  return ptr->ammo;
}

int Weapon::delay()
{
  return ptr->delay;
}

int Weapon::damage()
{
  return ptr->damage;
}

int Weapon::verticalSplashDamage()
{
  return ptr->vrtSplashDam;
}

int Weapon::horizontalSplashDamage()
{
  return ptr->horSplashDam;
}

int Weapon::horizontalSplashRadius()
{
  return ptr->horSplashRad;
}

//Airstrike
Airstrike::Airstrike()
{

}

Airstrike::Airstrike(_Airstrike* ptr) : ptr(ptr)
{

}

int Airstrike::id()
{
  return ptr->id;
}

int Airstrike::x()
{
  return ptr->x;
}

int Airstrike::y()
{
  return ptr->y;
}

int Airstrike::delay()
{
  return ptr->delay;
}

int Airstrike::itemID()
{
  return ptr->itemid;
}

//Spawnzone
Spawnzone::Spawnzone()
{

}

Spawnzone::Spawnzone(_Spawnzone* ptr) : ptr(ptr)
{

}

int Spawnzone::id()
{
  return ptr->id;
}

int Spawnzone::x1()
{
  return ptr->x1;
}

int Spawnzone::y1()
{
  return ptr->y1;
}

int Spawnzone::x2()
{
  return ptr->x2;
}

int Spawnzone::y2()
{
  return ptr->y2;
}

//Human
Human::Human()
{

}

Human::Human(_Human* ptr) : ptr(ptr)
{

}

int Human::id()
{
  return ptr->id;
}

int Human::x()
{
  return ptr->x;
}

int Human::y()
{
  return ptr->y;
}

int Human::hp()
{
  return ptr->hp;
}

int Human::maxHp()
{
  return ptr->maxhp;
}

int Human::moves()
{
  return ptr->moves;
}

int Human::maxMoves()
{
  return ptr->maxmoves;
}

int Human::weaponID()
{
  return ptr->weaponid;
}

int Human::sightRadius()
{
  return ptr->sight;
}

int Human::ownerID()
{
  return ptr->ownerid;
}

float Human::idleTime()
{
  return ptr->idletime;
}

int Human::ammo()
{
  return ptr->ammo;
}

int Human::specialty()
{
  return ptr->specialty;
}

void Human::move(int x, int y)
{
  humanMove(ptr, x, y);
}

void Human::attack(const Zombie& target)
{
  humanAttack(ptr, target.ptr);
}

void Human::attack(const Wall& target)
{
  humanAttackWall(ptr, target.ptr);
}

void Human::attack(const Crate& target)
{
  humanAttackCrate(ptr, target.ptr);
}

void Human::attack(int x, int y)
{
  humanAttackGround(ptr, x, y);
}

void Human::grab(const Crate& target)
{
  humanGrab(ptr, target.ptr);
}

void Human::throwWeapon(int x, int y)
{
  humanThrowWeapon(ptr, x, y);
}

void Human::throwAmmo(int x, int y, int quantity)
{
  humanThrowAmmo(ptr, x, y, quantity);
}

void Human::build(int x, int y)
{
  humanBuild(ptr, x, y);
}

void Human::callWeaponDrop(const Weapon& weapon, int x, int y)
{
  humanCallWeapon(weapon.ptr, x, y);
}

void Human::callAmmoDrop(int x, int y)
{
  humanCallAmmo(x,y);
}

void Human::spawn(const Weapon& weapon, int x, int y)
{
  humanSpawn(weapon.ptr, x, y);
}

//Zombie
Zombie::Zombie()
{

}

Zombie::Zombie(_Zombie* ptr) : ptr(ptr)
{

}

int Zombie::id()
{
  return ptr->id;
}

int Zombie::x()
{
  return ptr->x;
}

int Zombie::y()
{
  return ptr->y;
}

int Zombie::hp()
{
  return ptr->hp;
}

int Zombie::maxHp()
{
  return ptr->maxhp;
}

int Zombie::moves()
{
  return ptr->moves;
}

int Zombie::maxMoves()
{
  return ptr->maxmoves;
}

int Zombie::weaponID()
{
  return ptr->weaponid;
}

int Zombie::sightRadius()
{
  return ptr->sight;
}

int Zombie::ownerID()
{
  return ptr->ownerid;
}

float Zombie::idleTime()
{
  return ptr->idletime;
}

int Zombie::facing()
{
  return ptr->facing;
}

float Zombie::smell()
{
  return ptr->smell;
}

void Zombie::move(int x, int y)
{
  zombieMove(ptr, x, y);
}

void Zombie::attack(const Human& target)
{
  zombieAttack(ptr, target.ptr);
}

void Zombie::attack(const Wall& target)
{
  zombieAttackWall(ptr, target.ptr);
}

void Zombie::attack(int x, int y)
{
  zombieAttackGround(ptr, x, y);
}

void Zombie::eat(const Zombie& target)
{
  zombieEat(ptr, target.ptr);
}

void Zombie::turn(int direction)
{
  zombieTurn(ptr, direction);
}

void Zombie::spawn(const Spawnzone& spawn, int facing)
{
  zombieSpawn(spawn.ptr, facing);
}
