#include <string>
#include <cstring>
#include <iostream>
#include <sstream>

#include "game.h"
#include "network.h"
#include "structures.h"

#include "sexp/sexp.h"
#include "sexp/sexp_ops.h"

using namespace std;

static int turnNumber = 0;
static bool human = false;
static float airstrikesReady = 0;
static int zombiesReady = 0;
static int humansReady = 0;
static int zombieCap = 0;

static _Wall* walls = NULL;
static int wallCount = 0;

static _Crate* crates = NULL;
static int crateCount = 0;

static _Weapon* weapons = NULL;
static int weaponCount = 0;

static _Human* humans = NULL;
static int humanCount = 0;

static _Zombie* zombies = NULL;
static int zombieCount = 0;

static _Airstrike* airstrikes = NULL;
static int airstrikeCount = 0;

static _Spawnzone* spawnzones = NULL;
static int spawnzoneCount = 0;

static int socket;

void login(int s, const char* username, const char* password)
{
  socket = s;
  
  string expr = "(login \"";
  expr += username;
  expr += "\" \"";
  expr += password;
  expr +="\")";
  
  send_string(socket, expr.c_str());
}

void createGame()
{
  human = true;
  send_string(socket, "(create-game)");
}

void joinGame(int id)
{
  human = false;
  stringstream expr;
  expr << "(join-game " << id << " zombie)";
  send_string(socket, expr.str().c_str());
  send_string(socket, "(game-start)");
}

void endTurn()
{
  send_string(socket, "(end-turn)");
}

void getStatus()
{
  send_string(socket, "(game-status)");
}

bool isHuman()
{
  return human;
}

bool humanMove(_Human* unit, int x, int y)
{
  stringstream expr;
  expr << "(game-move " << unit->id << " " << x << " " << y << ")";
  send_string(socket, expr.str().c_str());
  return true;
}

bool humanAttack(_Human* unit, _Zombie* target)
{
  stringstream expr;
  expr << "(game-attack " << unit->id << " " << target->id << ")";
  send_string(socket, expr.str().c_str());
  return true;
}

bool humanAttackWall(_Human* unit, _Wall* target)
{
  stringstream expr;
  expr << "(game-attack " << unit->id << " " << target->id << ")";
  send_string(socket, expr.str().c_str());
  return true;
}

bool humanAttackCrate(_Human* unit, _Crate* target)
{
  stringstream expr;
  expr << "(game-attack " << unit->id << " " << target->id << ")";
  send_string(socket, expr.str().c_str());
  return true;
}

bool humanAttackGround(_Human* unit, int x, int y)
{
  stringstream expr;
  expr << "(game-attack-ground " << unit->id << " " << x << " " << y << ")";
  send_string(socket, expr.str().c_str());
  return true;
}

bool humanGrab(_Human* unit, _Crate* target)
{
  stringstream expr;
  expr << "(game-grab " << unit->id << " " << target->id << ")";
  send_string(socket, expr.str().c_str());
  return true;
}
bool humanThrowWeapon(_Human* unit, int x, int y)
{
  stringstream expr;
  expr << "(game-throw " << unit->id << " " << unit->weaponid << x << " " << y << " 1)";
  send_string(socket, expr.str().c_str());
  return true;
}

bool humanThrowAmmo(_Human* unit, int x, int y, int quantity)
{
  stringstream expr;
  expr << "(game-throw " << unit->id << " -1 " << x << " " << y << " " << quantity << ")";
  send_string(socket, expr.str().c_str());
  return true;
}
bool humanBuild(_Human* unit, int x, int y)
{
  stringstream expr;
  expr << "(game-build " << unit->id << " " << x << " " << y << ")";
  send_string(socket, expr.str().c_str());
  return true;
}

bool humanCallWeapon(_Weapon* weapon, int x, int y)
{
  stringstream expr;
  expr << "(game-airstrike " << weapon->id << " " << x << " " << y << ")";
  send_string(socket, expr.str().c_str());
  return true;
}

bool humanCallAmmo(int x, int y)
{
  stringstream expr;
  expr << "(game-airstrike -1 " << x << " " << y << ")";
  send_string(socket, expr.str().c_str());
  return true;
}

bool humanSpawn(_Weapon* weapon, int x, int y)
{
  stringstream expr;
  expr << "(game-human " << x << " " << y << " " << weapon->id << ")";
  send_string(socket, expr.str().c_str());
  return true;
}

bool zombieMove(_Zombie* unit, int x, int y)
{
  stringstream expr;
  expr << "(game-move " << unit->id << " " << x << " " << y << ")";
  send_string(socket, expr.str().c_str());
  return true;
}

bool zombieAttack(_Zombie* unit, _Human* target)
{
  stringstream expr;
  expr << "(game-attack " << unit->id << " " << target->id <<  ")";
  send_string(socket, expr.str().c_str());
  return true;
}

bool zombieAttackWall(_Zombie* unit, _Wall* target)
{
  stringstream expr;
  expr << "(game-attack " << unit->id << " " << target->id << ")";
  send_string(socket, expr.str().c_str());
  return true;
}

bool zombieAttackGround(_Zombie* unit, int x, int y)
{
  stringstream expr;
  expr << "(game-attack-ground " << unit->id << " " << x << " " << y << ")";
  send_string(socket, expr.str().c_str());
  return true;
}

bool zombieEat(_Zombie* unit, _Zombie* target)
{
  stringstream expr;
  expr << "(game-eat " << unit->id << " " << target->id << ")";
  send_string(socket, expr.str().c_str());
  return true;
}

bool zombieTurn(_Zombie* unit, int direction)
{
  stringstream expr;
  expr << "(game-turn " << unit->id << " " << direction << ")";
  send_string(socket, expr.str().c_str());
  return true;
}

bool zombieSpawn(_Spawnzone* spawn, int facing)
{
  stringstream expr;
  expr << "(game-zombie " << spawn->id << " " << facing << ")";
  send_string(socket, expr.str().c_str());
  return true;
}

int networkLoop(int socket)
{
  while(true)
  {
    sexp_t* expression, *sub;
    expression = extract_sexpr(rec_string(socket));
    expression = expression->list;
    if(expression->val != NULL && strcmp(expression->val, "game-over") == 0)
    {
      return 0;
    }
    else if(expression->val != NULL && strcmp(expression->val, "status") == 0)
    {
      while(expression->next != NULL)
      {
        expression = expression->next;
        sub = expression->list;
        if(string(sub->val) == "Game")
        {
          sub = sub->next;
          turnNumber = atoi(sub->val);
          sub = sub->next;
          humansReady = atoi(sub->val);
          sub = sub->next;
          airstrikesReady = atof(sub->val);
          sub = sub->next;
          zombiesReady = atoi(sub->val);
          sub = sub->next;
          zombieCap = atoi(sub->val);
        }
        else if(string(sub->val) == "Weapons")
        {
          weaponCount = sexp_list_length(expression)-1; //-1 for the header
          if(weapons)
            delete[] weapons;
          weapons = new _Weapon[weaponCount];
          for(int i = 0; i < weaponCount; i++)
          {
            sub = sub->next;
            sexp_t* weapon = sub->list->next;
            weapons[i].id = atoi(weapon->val);
            weapon = weapon->next;
            weapons[i].name = new char[strlen(weapon->val)];
            strncpy(weapons[i].name, weapon->val, strlen(weapon->val));
            weapon = weapon->next;
            weapons[i].range = atoi(weapon->val);
            weapon = weapon->next;
            weapons[i].ammo = atoi(weapon->val);
            weapon = weapon->next;
            weapons[i].delay = atoi(weapon->val);
            weapon = weapon->next;
            weapons[i].damage = atoi(weapon->val);
            weapon = weapon->next;
            weapons[i].vrtSplashDam = atoi(weapon->val);
            weapon = weapon->next;
            weapons[i].horSplashDam = atoi(weapon->val);
            weapon = weapon->next;
            weapons[i].horSplashRad = atoi(weapon->val);
          }
        }
        else if(string(sub->val) == "Walls")
        {
          wallCount = sexp_list_length(expression)-1; //-1 for the header
          if(walls)
            delete[] walls;
          walls = new _Wall[wallCount];
          for(int i = 0; i < wallCount; i++)
          {
            sub = sub->next;
            sexp_t* wall = sub->list->next;
            walls[i].id = atoi(wall->val);
            wall = wall->next;
            walls[i].x = atoi(wall->val);
            wall = wall->next;
            walls[i].y = atoi(wall->val);
            wall = wall->next;
            walls[i].hp = atoi(wall->val);
            wall = wall->next;
            walls[i].maxhp = atoi(wall->val);
          }
        }
        else if(string(sub->val) == "Crates")
        {
          crateCount = sexp_list_length(expression)-1; //-1 for the header
          if(crates)
            delete[] crates;
          crates = new _Crate[crateCount];
          for(int i = 0; i < crateCount; i++)
          {
            sub = sub->next;
            sexp_t* crate = sub->list->next;
            crates[i].id = atoi(crate->val);
            crate = crate->next;
            crates[i].x = atoi(crate->val);
            crate = crate->next;
            crates[i].y = atoi(crate->val);
            crate = crate->next;
            crates[i].hp = atoi(crate->val);
            crate = crate->next;
            crates[i].maxhp = atoi(crate->val);
            crate = crate->next;
            crates[i].content = atoi(crate->val);
          }
        }
        
        //("Human" id x y curHealth overHeal moves maxMoves weaponID sight ownerID idletime ammo specialty)
        else if(string(sub->val) == "Humans")
        {
          humanCount = sexp_list_length(expression)-1;
          if(humans)
            delete[] humans;
          humans = new _Human[humanCount];
          for(int i=0; i < humanCount; ++i)
          {
            sub = sub->next;
            sexp_t* human = sub->list->next;
            humans[i].id = atoi(human->val);
            human = human->next;
            humans[i].x = atoi(human->val);
            human = human->next;
            humans[i].y = atoi(human->val);
            human = human->next;
            humans[i].hp = atoi(human->val);
            human = human->next;
            humans[i].maxhp = atoi(human->val);
            human = human->next;
            humans[i].moves = atoi(human->val);
            human = human->next;
            humans[i].maxmoves = atoi(human->val);
            human = human->next;
            humans[i].weaponid = atoi(human->val);
            human = human->next;
            humans[i].sight = atoi(human->val);
            human = human->next;
            humans[i].ownerid = atoi(human->val);
            human = human->next;
            humans[i].idletime = atof(human->val);
            human = human->next;
            humans[i].ammo = atoi(human->val);
            human = human->next;
            humans[i].specialty = atoi(human->val);
          }
        }
        
        //("Zombie" id x y curHealth overHeal moves maxMoves weaponID sight ownerID idletime facing smell)
        else if(string(sub->val) == "Zombies")
        {
          zombieCount = sexp_list_length(expression)-1;
          if(zombies)
            delete[] zombies;
          zombies = new _Zombie[zombieCount];
          for(int i=0; i < zombieCount; ++i)
          {
            sub = sub->next;
            sexp_t* zombie = sub->list->next;
            zombies[i].id = atoi(zombie->val);
            zombie = zombie->next;
            zombies[i].x = atoi(zombie->val);
            zombie = zombie->next;
            zombies[i].y = atoi(zombie->val);
            zombie = zombie->next;
            zombies[i].hp = atoi(zombie->val);
            zombie = zombie->next;
            zombies[i].maxhp = atoi(zombie->val);
            zombie = zombie->next;
            zombies[i].moves = atoi(zombie->val);
            zombie = zombie->next;
            zombies[i].maxmoves = atoi(zombie->val);
            zombie = zombie->next;
            zombies[i].weaponid = atoi(zombie->val);
            zombie = zombie->next;
            zombies[i].sight = atoi(zombie->val);
            zombie = zombie->next;
            zombies[i].ownerid = atoi(zombie->val);
            zombie = zombie->next;
            zombies[i].idletime = atof(zombie->val);
            zombie = zombie->next;
            zombies[i].facing = atoi(zombie->val);
            zombie = zombie->next;
            zombies[i].smell = atof(zombie->val);
          }
        }
        
        //("Spawn" id x1 y1 x2 y2)
        else if(string(sub->val) == "Spawns")
        {
          spawnzoneCount = sexp_list_length(expression)-1; //-1 for the header
          if(spawnzones)
            delete[] spawnzones;
          spawnzones = new _Spawnzone[spawnzoneCount];
          for(int i = 0; i < spawnzoneCount; i++)
          {
            sub = sub->next;
            sexp_t* spawn = sub->list->next;
            spawnzones[i].id = atoi(spawn->val);
            spawn = spawn->next;
            spawnzones[i].x1 = atoi(spawn->val);
            spawn = spawn->next;
            spawnzones[i].y1 = atoi(spawn->val);
            spawn = spawn->next;
            spawnzones[i].x2 = atoi(spawn->val);
            spawn = spawn->next;
            spawnzones[i].y2 = atoi(spawn->val);
          }
        }
        
        //("Airstrike" id x y delay itemID)
        else if(string(sub->val) == "Airstrikes")
        {
          airstrikeCount = sexp_list_length(expression)-1; //-1 for the header
          if(airstrikes)
            delete[] airstrikes;
          airstrikes = new _Airstrike[airstrikeCount];
          for(int i = 0; i < airstrikeCount; i++)
          {
            sub = sub->next;
            sexp_t* airstrike = sub->list->next;
            airstrikes[i].id = atoi(airstrike->val);
            airstrike = airstrike->next;
            airstrikes[i].x = atoi(airstrike->val);
            airstrike = airstrike->next;
            airstrikes[i].y = atoi(airstrike->val);
            airstrike = airstrike->next;
            airstrikes[i].delay = atoi(airstrike->val);
            airstrike = airstrike->next;
            airstrikes[i].itemid = atoi(airstrike->val);
          }
        }
      }
      if(human && (turnNumber & 1))
        return 1;
      else if(!human && !(turnNumber & 1))
        return 1;
    }
  }
  return 0;
}

//the following was automagically generated to play nicely with other languages
int getTurnNumber()
{
  return turnNumber;
}
float getAirstrikesReady()
{
  return airstrikesReady;
}
int getZombiesReady()
{
  return zombiesReady;
}
int getHumansReady()
{
  return humansReady;
}
int getZombieCap()
{
  return zombieCap;
}

_Wall* getWalls()
{
  return walls;
}
int getWallCount()
{
  return wallCount;
}

_Crate* getCrates()
{
  return crates;
}
int getCrateCount()
{
  return crateCount;
}

_Weapon* getWeapons()
{
  return weapons;
}
int getWeaponCount()
{
  return weaponCount;
}

_Human* getHumans()
{
  return humans;
}
int getHumanCount()
{
  return humanCount;
}

_Zombie* getZombies()
{
  return zombies;
}
int getZombieCount()
{
  return zombieCount;
}

_Airstrike* getAirstrikes()
{
  return airstrikes;
}
int getAirstrikeCount()
{
  return airstrikeCount;
}

_Spawnzone* getSpawnzones()
{
  return spawnzones;
}
int getSpawnzoneCount()
{
  return spawnzoneCount;
}
