//Copyright (C) 2009 - Missouri S&T ACM AI Team
//Please do not modify this file while building your AI
//See AI.h & AI.cpp for that
#pragma warning(disable : 4996)

#include <string>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <fstream>

#include "game.h"
#include "network.h"
#include "structures.h"

#include "sexp/sexp.h"
#include "sexp/sexp_ops.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#ifdef WIN32
//Doh, namespace collision.
namespace Windows
{
    #include <Windows.h>
};
#else
#include <unistd.h>
#endif

using namespace std;

int    _argc;
char** _argv;
static int turnNumber = 0;
static bool human = false;
static float airstrikesReady = 0;
static int zombiesReady = 0;
static int humansReady = 0;
static int zombieCap = 0;
static char humanTeamName[100]="Unknown";
static char zombieTeamName[100]="Unknown";
static char logID[100]="";
static int mapSize = 0;
static float humanTime = 0;
static float zombieTime = 0;

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

static int distance(int x1, int y1, int x2, int y2)
{
  int dx = x2-x1;
  int dy = y2-y1;
  if(dx * dy > 0)
    return abs(dx) + abs(dy);
  return abs(dx) > abs(dy) ? abs(dx) : abs(dy);
}

static void* getPtr(int id)
{
  for(int i = 0; i < wallCount; i++)
    if(walls[i].id == id)
      return walls+i;
  for(int i = 0; i < crateCount; i++)
    if(crates[i].id == id)
      return crates+i;
  for(int i = 0; i < weaponCount; i++)
    if(weapons[i].id == id)
      return weapons+i;
  for(int i = 0; i < humanCount; i++)
    if(humans[i].id == id)
      return humans+i;
  for(int i = 0; i < zombieCount; i++)
    if(zombies[i].id == id)
      return zombies+i;
  for(int i = 0; i < airstrikeCount; i++)
    if(airstrikes[i].id == id)
      return airstrikes+i;
  for(int i = 0; i < spawnzoneCount; i++)
    if(spawnzones[i].id == id)
      return spawnzones+i;
  return NULL;
}

static void attack(_Weapon* weapon, int x, int y)
{
  //vertical damage
  for(int i = 0; i < wallCount; i++)
  {
    if(walls[i].x == x && walls[i].y == y)
    {
      walls[i].hp -= weapon->vrtSplashDam;
    }
  }
  for(int i = 0; i < crateCount; i++)
  {
    if(crates[i].x == x && crates[i].y == y)
    {
      crates[i].hp -= weapon->vrtSplashDam;
      if(crates[i].hp < 1)
        crates[i].content = -2;
    }
  }
  for(int i = 0; i < humanCount; i++)
  {
    if(humans[i].x == x && humans[i].y == y)
    {
      humans[i].hp -= weapon->vrtSplashDam;
    }
  }
  for(int i = 0; i < zombieCount; i++)
  {
    if(zombies[i].x == x && zombies[i].y == y)
    {
      zombies[i].hp -= weapon->vrtSplashDam;
    }
  }

  //horizontal damage
  for(int i = 0; i < wallCount; i++)
  {
    if(distance(walls[i].x, walls[i].y, x, y) <= weapon-> horSplashRad)
    {
      walls[i].hp -= weapon->horSplashDam;
    }
  }
  for(int i = 0; i < crateCount; i++)
  {
    if(distance(crates[i].x, crates[i].y, x, y) <= weapon-> horSplashRad)
    {
      crates[i].hp -= weapon->horSplashDam;;
      if(crates[i].hp < 1)
        crates[i].content = -2;
    }
  }
  for(int i = 0; i < humanCount; i++)
  {
    if(distance(humans[i].x, humans[i].y, x, y) <= weapon-> horSplashRad)
    {
      humans[i].hp -= weapon->horSplashDam;
    }
  }
  for(int i = 0; i < zombieCount; i++)
  {
    if(distance(zombies[i].x, zombies[i].y, x, y) <= weapon-> horSplashRad)
    {
      zombies[i].hp -= weapon->horSplashDam;
    }
  }
}

static void attack(_Weapon* weapon, int id)
{
  for(int i = 0; i < wallCount; i++)
  {
    if(walls[i].id == id)
    {
      walls[i].hp -= weapon->damage;
      attack(weapon, walls[i].x, walls[i].y);
    }
  }

  for(int i = 0; i < crateCount; i++)
  {
    if(crates[i].id == id)
    {
      crates[i].hp -= weapon->damage;
      if(crates[i].hp < 1)
        crates[i].content = -2;
      attack(weapon, crates[i].x, crates[i].y);
    }
  }

  for(int i = 0; i < humanCount; i++)
  {
    if(humans[i].id == id)
    {
      humans[i].hp -= weapon->damage;
      attack(weapon, humans[i].x, humans[i].y);
    }
  }

  for(int i = 0; i < zombieCount; i++)
  {
    if(zombies[i].id == id)
    {
      zombies[i].hp -= weapon->damage;
      attack(weapon, zombies[i].x, zombies[i].y);
    }
  }
}




DLLEXPORT bool login(int s, const char* username, const char* password)
{
  socket = s;

  string expr = "(login \"";
  expr += username;
  expr += "\" \"";
  expr += password;
  expr +="\")";

  send_string(socket, expr.c_str());

  sexp_t* expression;

  expression = extract_sexpr(rec_string(socket));
  expression = expression->list;
  if(expression->val == NULL || strcmp(expression->val, "login-accepted") != 0)
  {
    cerr << "Unable to login to server" << endl;
    return false;
  }
  return true;
}

DLLEXPORT void createGame()
{
  human = true;
  send_string(socket, "(create-game)");
}

DLLEXPORT void joinGame(int id)
{
  human = false;
  stringstream expr;
  expr << "(join-game " << id << " zombie)";
  send_string(socket, expr.str().c_str());
  send_string(socket, "(game-start)");
}

DLLEXPORT void endTurn()
{
  send_string(socket, "(end-turn)");
}

DLLEXPORT void getStatus()
{
  send_string(socket, "(game-status)");
}

DLLEXPORT bool isHuman()
{
  return human;
}

DLLEXPORT bool humanMove(_Human* unit, int x, int y)
{
  stringstream expr;
  expr << "(game-move " << unit->id << " " << x << " " << y << ")";
  send_string(socket, expr.str().c_str());

  unit->x=x;
  unit->y=y;
  unit->moves--;
  return true;
}

DLLEXPORT bool humanAttack(_Human* unit, _Zombie* target)
{
  stringstream expr;
  expr << "(game-attack " << unit->id << " " << target->id << ")";
  send_string(socket, expr.str().c_str());
  attack((_Weapon*)getPtr(unit->weaponid), target->id);
  unit->idletime -= ((_Weapon*)getPtr(unit->weaponid))->delay;
  unit->idletime = unit->idletime > 1 ? 1 : unit->idletime;
  return true;
}

DLLEXPORT bool humanAttackWall(_Human* unit, _Wall* target)
{
  stringstream expr;
  expr << "(game-attack " << unit->id << " " << target->id << ")";
  send_string(socket, expr.str().c_str());
  attack((_Weapon*)getPtr(unit->weaponid), target->id);
  unit->idletime -= ((_Weapon*)getPtr(unit->weaponid))->delay;
  unit->idletime = unit->idletime > 1 ? 1 : unit->idletime;
  return true;
}

DLLEXPORT bool humanAttackCrate(_Human* unit, _Crate* target)
{
  stringstream expr;
  expr << "(game-attack " << unit->id << " " << target->id << ")";
  send_string(socket, expr.str().c_str());
  attack((_Weapon*)getPtr(unit->weaponid), target->id);
  unit->idletime -= ((_Weapon*)getPtr(unit->weaponid))->delay;
  unit->idletime = unit->idletime > 1 ? 1 : unit->idletime;
  return true;
}

DLLEXPORT bool humanAttackGround(_Human* unit, int x, int y)
{
  stringstream expr;
  expr << "(game-attack-ground " << unit->id << " " << x << " " << y << ")";
  send_string(socket, expr.str().c_str());
  attack((_Weapon*)getPtr(unit->weaponid), x, y);
  unit->idletime -= ((_Weapon*)getPtr(unit->weaponid))->delay;
  unit->idletime = unit->idletime > 1 ? 1 : unit->idletime;
  return true;
}

DLLEXPORT bool humanGrab(_Human* unit, _Crate* target)
{
  stringstream expr;
  expr << "(game-grab " << unit->id << " " << target->id << ")";
  send_string(socket, expr.str().c_str());
  if(target->content == -1)
  {
    unit->ammo += target->hp;
  }
  else if(target->content > -1)
  {
    unit->weaponid = target->content;
  }
  target->content = -2;
  return true;
}

DLLEXPORT bool humanGive(_Human* unit, _Human* target, int quantity)
{
  stringstream expr;
  expr << "(game-give " << unit->id << " " << target->id << " " << quantity << ")";
  send_string(socket, expr.str().c_str());
  unit->ammo -= quantity;
  target->ammo += quantity;
  return true;
}

DLLEXPORT bool humanThrowWeapon(_Human* unit, int x, int y)
{
  stringstream expr;
  expr << "(game-throw " << unit->id << " " << unit->weaponid << " " << x << " " << y << " 1)";
  send_string(socket, expr.str().c_str());
  return true;
}

DLLEXPORT bool humanThrowAmmo(_Human* unit, int x, int y, int quantity)
{
  stringstream expr;
  expr << "(game-throw " << unit->id << " -1 " << x << " " << y << " " << quantity << ")";
  send_string(socket, expr.str().c_str());
  unit->ammo -= quantity;
  return true;
}

DLLEXPORT bool humanBuild(_Human* unit, int x, int y)
{
  stringstream expr;
  expr << "(game-build " << unit->id << " " << x << " " << y << ")";
  send_string(socket, expr.str().c_str());
  for(int i = 0; i < wallCount; i++)
  {
    if(walls[i].x == x && walls[i].y == y)
    {
      walls[i].hp += 5;
      break;
    }
  }
  unit->moves -= 1;
  return true;
}

DLLEXPORT bool humanCallWeapon(_Weapon* weapon, int x, int y)
{
  stringstream expr;
  expr << "(game-airstrike " << weapon->id << " " << x << " " << y << ")";
  send_string(socket, expr.str().c_str());
  airstrikesReady -= 1;
  return true;
}

DLLEXPORT bool humanCallAmmo(int x, int y)
{
  stringstream expr;
  expr << "(game-airstrike -1 " << x << " " << y << ")";
  send_string(socket, expr.str().c_str());
  airstrikesReady -= 1;
  return true;
}

DLLEXPORT bool humanSpawn(_Weapon* weapon, int x, int y)
{
  stringstream expr;
  expr << "(game-human " << x << " " << y << " " << weapon->id << ")";
  send_string(socket, expr.str().c_str());
  humansReady--;
  return true;
}

DLLEXPORT bool zombieMove(_Zombie* unit)
{
  int x = unit->x;
  int y = unit->y;
  int  facing = unit-> facing;
  if(facing == 0 || facing == 5) x+= 1;
  if(facing == 2 || facing == 3) x-= 1;
  if(facing == 1 || facing == 2) y += 1;
  if(facing == 4 || facing == 5) y -= 1;
  stringstream expr;
  expr << "(game-move " << unit->id << " " << x << " " << y << ")";
  send_string(socket, expr.str().c_str());
  unit->x=x;
  unit->y=y;
  unit->moves--;
  return true;
}

DLLEXPORT bool zombieAttack(_Zombie* unit, _Human* target)
{
  stringstream expr;
  expr << "(game-attack " << unit->id << " " << target->id <<  ")";
  send_string(socket, expr.str().c_str());
  attack((_Weapon*)getPtr(unit->weaponid), target->id);
  unit->moves -= 1;
  return true;
}

DLLEXPORT bool zombieAttackWall(_Zombie* unit, _Wall* target)
{
  stringstream expr;
  expr << "(game-attack " << unit->id << " " << target->id << ")";
  send_string(socket, expr.str().c_str());
  attack((_Weapon*)getPtr(unit->weaponid), target->id);
  unit->moves -= 1;
  return true;
}

DLLEXPORT bool zombieAttackGround(_Zombie* unit, int x, int y)
{
  stringstream expr;
  expr << "(game-attack-ground " << unit->id << " " << x << " " << y << ")";
  send_string(socket, expr.str().c_str());
  attack((_Weapon*)getPtr(unit->weaponid), x, y);
  unit->moves -= 1;
  return true;
}

DLLEXPORT bool zombieEat(_Zombie* unit, _Zombie* target)
{
  stringstream expr;
  expr << "(game-eat " << unit->id << " " << target->id << ")";
  send_string(socket, expr.str().c_str());
  unit->hp += target->hp * 2;
  if(unit->hp > unit->maxhp)
    unit->hp = unit->maxhp;
  unit->moves -= 1;
  return true;
}

DLLEXPORT bool zombieTurn(_Zombie* unit, int direction)
{
  stringstream expr;
  expr << "(game-turn " << unit->id << " " << direction << ")";
  send_string(socket, expr.str().c_str());
  unit->facing = unit->facing + direction % 6;
  unit->moves -= 1;
  return true;
}

DLLEXPORT bool zombieSpawn(_Spawnzone* spawn, int facing)
{
  stringstream expr;
  expr << "(game-zombie " << spawn->id << " " << facing << ")";
  send_string(socket, expr.str().c_str());
  zombiesReady--;
  return true;
}

DLLEXPORT int networkLoop(int socket, bool practice)
{
  while(true)
  {
    sexp_t* expression, *sub;
    expression = extract_sexpr(rec_string(socket));
    expression = expression->list;
    if(expression->val != NULL && strcmp(expression->val, "game-over") == 0)
    {
      char request[100]="(request-log ";
      strcat(request, logID);
      strcat(request, ")");
      send_string(socket, request);
      return 0;
    }
    else if(expression->val != NULL && strcmp(expression->val, "status") == 0)
    {
      humanCount = 0;
      if(humans)
        delete[] humans;
      humans = NULL;
      wallCount = 0;
      if(walls)
        delete[] walls;
      walls = NULL;
      zombieCount = 0;
      if(zombies)
        delete[] zombies;
      zombies = NULL;
      crateCount = 0;
      if(crates)
        delete[] crates;
      crates = NULL;
      airstrikeCount = 0;
      if(airstrikes)
        delete[] airstrikes;
      airstrikes = NULL;
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
          airstrikesReady = (float)atof(sub->val);
          sub = sub->next;
          zombiesReady = atoi(sub->val);
          sub = sub->next;
          zombieCap = atoi(sub->val);
          sub = sub->next;
          humanTime = (float)atof(sub->val);
          sub = sub->next;
          zombieTime = (float)atof(sub->val);
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
            weapons[i].name = new char[strlen(weapon->val)+1];
            strncpy(weapons[i].name, weapon->val, strlen(weapon->val));
            weapons[i].name[strlen(weapon->val)] = 0;
            weapon = weapon->next;
            weapons[i].range = atoi(weapon->val);
            weapon = weapon->next;
            weapons[i].ammo = atoi(weapon->val);
            weapon = weapon->next;
            weapons[i].delay = atof(weapon->val);
            weapon = weapon->next;
            weapons[i].damage = atoi(weapon->val);
            weapon = weapon->next;
            weapons[i].vrtSplashDam = atoi(weapon->val);
            weapon = weapon->next;
            weapons[i].horSplashDam = atoi(weapon->val);
            weapon = weapon->next;
            weapons[i].horSplashRad = atoi(weapon->val);
            weapon = weapon->next;
            weapons[i].canSpawnWith = (atoi(weapon->val) == 1);
            weapon = weapon->next;
            weapons[i].canCall = (atoi(weapon->val) == 1);
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
            humans[i].idletime = (float)atof(human->val);
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
            zombies[i].idletime = (float)atof(zombie->val);
            zombie = zombie->next;
            zombies[i].facing = atoi(zombie->val);
            zombie = zombie->next;
            zombies[i].smell = (float)atof(zombie->val);
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
      if(turnNumber == 1)
        return 1;
      else if(human && (turnNumber & 1))
        return 1;
      else if(!human && !(turnNumber & 1))
        return 1;
    }
    else if(expression->val != NULL && strcmp(expression->val, "log") == 0)
    {
      ofstream out;
      char filename[100];
      expression = expression->next;
      strcpy(filename, expression->val);
      strcat(filename, ".gamelog");
      expression = expression->next;
      out.open(filename);
      if (out.good())
        out.write(expression->val, strlen(expression->val));
      else
        cerr << "Error : Could not create log." << endl;
      out.close();
      return 0;
    }
    else if(expression->val != NULL && strcmp(expression->val, "ident") == 0)
    {
    //("ident" (("0" "MuesAI" "Stephen Mues" "human") ("1" "MuesAI" "Stephen Mues" "zombie")) "0")
      expression = expression->next;
      sub = expression->list;
      strcpy(humanTeamName, sub->list->next->next->val);
      strcpy(zombieTeamName, sub->next->list->next->next->val);
      expression = expression->next;
      strcpy(logID, expression->val);
      expression = expression->next;
      mapSize = atoi(expression->val);
    }
    else if(expression->val != NULL && strcmp(expression->val, "game-accepted")==0)
    {
      char gameID[30];

      expression = expression->next;
      strcpy(gameID, expression->val);
      cout << "Created game " << gameID << endl;
      if (practice)
      {
#ifdef WIN32
        string binary = _argv[0];
        for(int i=1; i <= _argc; ++i)
        {
            binary += " ";
            binary += _argv[i];
        }
        /*
        Windows::CreateProcessA(binary.c_str(),
                                0,
                                0,
                                0,//not opt
                                0,//not opt
                                0,
                                0,
                                0,
                                0,//not opt
                                0 //not opt
                                );
                                */
#else
        string binary = "./";
        binary += _argv[0];
        pid_t child_pid;
        if(fork() == 0)
        {
          execl(binary.c_str(), _argv[0], _argv[1], gameID, (char *)0);
          //cout << "EXECL FAIL" << endl;
          _exit(1);
        }
#endif
      }
    }
    else if(expression->val != NULL && strstr(expression->val, "denied"))
    {
      cout << expression->val << endl;
      cout << expression->next->val << endl;
    }
  }
  return 0;
}

//the following was automagically generated to play nicely with other languages
DLLEXPORT int getTurnNumber()
{
  return turnNumber;
}
DLLEXPORT float getAirstrikesReady()
{
  return airstrikesReady;
}
DLLEXPORT int getZombiesReady()
{
  return zombiesReady;
}
DLLEXPORT int getHumansReady()
{
  return humansReady;
}
DLLEXPORT int getZombieCap()
{
  return zombieCap;
}

DLLEXPORT _Wall* getWalls(int num)
{
  return walls + num;
}
DLLEXPORT int getWallCount()
{
  return wallCount;
}

DLLEXPORT _Crate* getCrates(int num)
{
  return crates + num;
}
DLLEXPORT int getCrateCount()
{
  return crateCount;
}

DLLEXPORT _Weapon* getWeapons(int num)
{
  return weapons + num;
}
DLLEXPORT int getWeaponCount()
{
  return weaponCount;
}

DLLEXPORT _Human* getHumans(int num)
{
  return humans + num;
}
DLLEXPORT int getHumanCount()
{
  return humanCount;
}

DLLEXPORT _Zombie* getZombies(int num)
{
  return zombies + num;
}
DLLEXPORT int getZombieCount()
{
  return zombieCount;
}

DLLEXPORT _Airstrike* getAirstrikes(int num)
{
  return airstrikes + num;
}
DLLEXPORT int getAirstrikeCount()
{
  return airstrikeCount;
}

DLLEXPORT _Spawnzone* getSpawnzones(int num)
{
  return spawnzones + num;
}
DLLEXPORT int getSpawnzoneCount()
{
  return spawnzoneCount;
}

DLLEXPORT char* getHumanTeamName()
{
  return humanTeamName;
}

DLLEXPORT char* getZombieTeamName()
{
  return zombieTeamName;
}

DLLEXPORT float getHumanTime()
{
  return humanTime;
}

DLLEXPORT float getZombieTime()
{
  return zombieTime;
}

DLLEXPORT int getMapSize()
{
  return mapSize;
}

