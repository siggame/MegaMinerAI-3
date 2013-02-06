"""

   Copyright (C) 2008 by Steven Wallace, Ben Muell, Stephen Mues
   snwallace@gmail.com, ben@benmurrell.com

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the
    Free Software Foundation, Inc.,
    59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
"""
"""
    File    : Game.py
    Purpose : This file defines the rules of the game.  To do this,
              several classes are created to represent the various objects
              that will appear in the game.  The most important class
              is the Game class.  Statements.py will provide a sort of
              directory for the server, linking the string expressions
              sent by the user to member functions of the desired Game object.

              All variables that we may want to change in order to balance
              the game have been placed in config files rather than
              hardcoded. This allows the Balancing team to adjust the game
              without having to learn all of the server code.
"""


##from Records import Records
import dircache
from config import configWeapons
from config import configHuman
from config import configClassDefaults
from ConfigParser import SafeConfigParser
import time
import calendar
import sys
sys.path.append("lexpr/")
from sexpr import sexpr2str
from db import Game as game_log

import random
import math

CONFIG = 'config/defaults.cfg'

MAP_FOLDER = "maps/"
MAP_LIST = "mapList.dat"

class GameObject:
    """
    All objects that appear in the game will inherit from this class.
    This class assigns each object a unique id.

    All objects in the game should specify a toList function first call's
    the toList function of its base class and then adds all of its relevant
    stats to the list. These stats will be sent in S-Expressions to the
    client whenever the player can see this object.

    All objects in the game should also specify a nextTurn function to be
    called at the beginning of each turn.
    """
    myType = "GameObject"
    
    def __init__(self, game):
        self.game = game
        self.id = game.nextid
        self.turnCreated = game.turnNum
        game.nextid += 1
        if game.nextid > game.maxid:
            game.nextid = 0

    def toList(self):
        list = [self.myType, self.id]
        #list = ['', self.id]
        return list

    def nextTurn(self):
        pass

class MappableObject(GameObject):
    """
    All objects that are associated with a single x, y location will
    inherit from this class.  This class provides functions for measuring
    distance and moving.  Some attributes about the map, such as the radius,
    are also stored as static members of this class.
    """
    myType = "MappableObject"
    _boundaryRadius = 13 #see defaults.cfg

    def __init__(self, game, x, y):
        GameObject.__init__(self, game)
        self.x = x
        self.y = y

    def nextTurn(self):
        GameObject.nextTurn(self)

    #def distanceTo(self, target):
    #def distanceTo(self, targetX, targetY):
    def distanceTo(self, arg1, arg2 = None):
        if (arg2 == None):
            targetX = arg1.x
            targetY = arg1.y
        else:
            targetX = arg1
            targetY = arg2
        return MappableObject.distance(self.x, self.y, targetX, targetY)
          
    def distance(startX, startY, targetX, targetY):
        dx = targetX - startX
        dy = targetY - startY
        if (dx * dy > 0):
            dis = abs(dx) + abs(dy)
        else:
            dis = max(abs(dx), abs(dy))
        return dis   

    #def move(self, target):
    #def move(self, targetX, targetY):
    def move(self, arg1, arg2=None):
        if (arg2==None):
            targetX = arg1.x
            targetY = arg1.y
        else:
            targetX = arg1
            targetY = arg2
        self.x = targetX
        self.y = targetY
        self._changed = True

    def toList(self):
        list = GameObject.toList(self)
        list.extend([self.x, self.y])
        return list

    def isInBounds(self):
        return MappableObject.inBounds(self.x, self.y)

    def inBounds(x, y):
        retVal = False
        #HEX MAP
        #if (MappableObject.distance(x, y, 0, 0) <= \
        #  MappableObject._boundaryRadius):
        #    retVal = True
        if (abs(x) <= MappableObject._boundaryRadius and \
            abs(y) <= MappableObject._boundaryRadius):
            retVal = True
        return retVal

    def mapKey(self):
        return MappableObject.toMapKey(self.x , self.y)

    def toMapKey(x, y):
        return (str(x) + ", " + str(y))

    toMapKey = staticmethod(toMapKey)

    def listRadius(x, y, rad):
        list = []
        if rad == 0:
            list.append(MappableObject.toMapKey(x, y))
        else:
            for dx in range(-rad, rad+1):
                for dy in range(-rad, rad+1):
                    if (MappableObject.distance(0, 0, dx, dy) <= rad):
                        list.append(MappableObject.toMapKey(x + dx, y+dy))
        return list

    distance = staticmethod(distance)
    inBounds = staticmethod(inBounds)
    listRadius = staticmethod(listRadius)


class SpawnZone(GameObject):
    """
    This class represents a parrellelgram, where zombies can choose to spawn.
    To prevent luck from influencing the outcome of the game, the hex
    locations in the parrellelgram are stored in a list.  The list 
    is shuffled in a simple pattern, and from then on it functions as a queue.
    When a location is popped off the front of the queue, it is added to the
    back of the queue.
    """
    myType = "Spawn"
    
    def __init__(self, game, x1, y1, x2, y2):
        GameObject.__init__(self, game)
        self.x1 = x1
        self.y1 = y1
        self.x2 = x2
        self.y2 = y2
        self.hexList = []
        self.createHexList() 
        self.scramble()

    def toList(self):
        list = GameObject.toList(self)
        list.extend([self.x1, self.y1, self.x2, self.y2])
        return list

    def createHexList(self):
        self.hexList.append([self.x1, self.y1])
        basePoint = [self.x1, self.y1]
        self.expand( 1, 0, basePoint)
        self.expand( 0, 1, basePoint)
        self.expand(-1, 1, basePoint)
        self.expand(-1, 0, basePoint)
        self.expand( 0,-1, basePoint)
        self.expand( 1,-1, basePoint)

    def expand(self, xOffset, yOffset, basePoint):
        addList = []
        for hex in self.hexList:
            addList.append([int(hex[0]), int(hex[1])])
        disAfter = MappableObject.distance(basePoint[0]+xOffset,\
                         basePoint[1] + yOffset, self.x2, self.y2)
        disBefore = MappableObject.distance(basePoint[0], basePoint[1], \
                                           self.x2, self.y2)
        while (disAfter < disBefore):
            for hex in addList:
                hex[0] = hex[0] + xOffset
                hex[1] = hex[1] + yOffset
            for hex in addList:
                self.hexList.append([int(hex[0]), int(hex[1])])
            basePoint[0] = basePoint[0] + xOffset
            basePoint[1] = basePoint[1] + yOffset
            disAfter = MappableObject.distance(basePoint[0]+xOffset,\
                         basePoint[1] + yOffset, self.x2, self.y2)
            disBefore = MappableObject.distance(basePoint[0], basePoint[1], \
                                                self.x2, self.y2)
            
    def scramble(self):
        leftList = []
        rightList = []
        for i in range(0, len(self.hexList)):
            if i <= (len(self.hexList)/2):
                leftList.append([int(self.hexList[i][0]), \
                                 int(self.hexList[i][1])])
            else:
                rightList.append([int(self.hexList[i][0]), \
                                 int(self.hexList[i][1])])
        leftList.reverse()
        self.hexList = []
        while len(leftList) + len(rightList) > 0:
            self.hexList.append(leftList.pop())
            if len(rightList) > 0:
                self.hexList.append(rightList.pop())

    def pop(self):
        retVal = []
        retVal = self.hexList.pop()
        self.hexList.insert(0, retVal)
        return retVal


class Airstrike(MappableObject):
    """
    An airstrike object represents a crate falling from the sky.
    Humans will periodically be awarded the opportunity to call for an
    airdrop on a location.  When they do, they create an instance of this 
    class.  After a set time, this airstrike causes vertical damage, 
    is removed from the game, and a crate is created.
    """
    myType = "Airstrike"
    _delay = 10   #see defaults.cfg
    _damage = 100 #see defaults.cfg
    _frequency = 0.05 #see defaults.cfg.  This value is added to the human
                      #team's score at the beginning of their turn.

    def __init__(self, game, x, y, contents):
        MappableObject.__init__(self, game, x, y)
        self.delay = Airstrike._delay
        self.contents = contents

    def nextTurn(self):
        MappableObject.nextTurn(self)
        self.delay -= 1

    def ready(self):
        return (self.delay <= 0)

    def move(self, x, y=None):
        return #Airstrikes can not move.

    def toList(self):
        list = MappableObject.toList(self)
        if isinstance(self.contents, WeaponType):
            itemID = self.contents.id
        else:
            itemID = self.contents
        list.extend([self.delay, itemID])
        return list

class HittableObject(MappableObject):
    """
    Any object that is destructible or targetable with weapons is an
    instance of this class.  This class contains any functions that
    are solely related to health, damage, or healing.
    """
    myType = "HittableObject"
    def __init__(self, game, x, y, maxHP, overheal):
        MappableObject.__init__(self, game, x, y)
        self.maxHP = maxHP
        self.hp = maxHP
        self.overheal = overheal

    def nextTurn(self):
        MappableObject.nextTurn(self)

    def toList(self):
        list = MappableObject.toList(self)
        list.extend([self.hp, int(self.maxHP*self.overheal)])
        return list

    def isDestroyed(self):
        destroyed = False
        if (self.hp <= 0):
            destroyed = True
        return destroyed

    def takeDamage(self, damage):
        self.hp -= damage
        if (self.hp > self.maxHP * self.overheal):
            self.hp = self.maxHP * self.overheal


class Wall(HittableObject):
    """
    Instances of this class represent walls that can be built by
    humans or destroyed by any unit.
    """
    myType = "Wall"
    _overheal = 10000.00 #see defaults.cfg
    _hpIncr = 50
    def __init__(self, game, x, y, maxHP):
        HittableObject.__init__(self, game, x, y, maxHP, Wall._overheal)

    def nextTurn(self):
         HittableObject.nextTurn(self)

    def toList(self):
        list = HittableObject.toList(self)
        return list

    def takeDamge(self, damage):
        #Walls can not be healed
        if (damage > 0):
            HittableObject.takeDamage(self, damage)


class Crate(HittableObject):
    """
    Crates can contain either weapons or ammo.
    If the crate contains a weapon,
      - contents will be a WeaponType instance.
      - So long as the crate is not destroyed, any amount of HP
         means the weapon is still functional and can be picked up.
    If the crate contains ammo, 
      - contents will be an integer
      - contents will equal Crate._ammoType.  
      - The amount of ammo is equal to the crate's HP.
    """
    myType = "Crate"
    _overheal = 1.00 #see defaults.cfg
    _hp = 100
    #constants
    _ammoType = -1
    
    def __init__(self, game, x, y, contents, hp = None):
        if hp is None:
            hp = Crate._hp
        HittableObject.__init__(self, game, x, y, hp, Crate._overheal)
        self.contents = contents #contents must be -2, -1, or a weapon object

    def nextTurn(self):
        HittableObject.nextTurn(self)

    def toList(self):
        list = HittableObject.toList(self)
        if isinstance(self.contents, WeaponType):
            list.extend([self.contents.id])
        else:
            list.extend([self.contents])
        return list

    def takeDamge(self, damage):
        #Crates can not be healed
        if (damage > 0):
            HittableObject.takeDamage(self, damage)

class Unit(HittableObject):   
    """ 
    Any object that is owned by a player, has a weapon, can see, 
    and can move inherits from this class.  This class handles functions 
    related to sight, idle time, ammo, weapons, moves, etc.
    idleTime represents the amount of time the unit has had to reload his
    weapon.  It is a bit confusing, but as it is written, it allows for 
    weapons to have a delay that is either a fraction of a turn or multiple
    turns and still function as intended.
    """
    myType = "Unit"
    def __init__(self, game, x, y, maxHP, overheal, owner, 
                 sight, maxMoves, weapon):
        HittableObject.__init__(self, game, x, y, maxHP, overheal)
        self.maxMoves = maxMoves
        self.moves = 0
        self.sight = sight
        self.weapon = weapon
        self.idleTime = .5 
        self.ammo = 0
        self.owner = owner

    def toList(self):
        list = HittableObject.toList(self)
        list.extend([self.moves, self.maxMoves, self.weapon.id, self.sight, 
                     self.owner.ID, self.idleTime])
        return list

    def nextTurn(self):
        HittableObject.nextTurn(self)
        self.moves = self.maxMoves
        self.idleTime += 0.5
    
    def canSee(self, target):
        visible = False
        if (self.distanceTo(target) <= sight):
            visible = True
        return visible

    def useTime(self, amount):
        success = (amount < self.idleTime)
        if (success):
            self.idleTime -= amount
            if (self.idleTime > 1):
                self.idleTime = 1
        return success

    #def move(self, target):
    #def move(self, targetX, targetY):
    def move(self, arg1, arg2=None):
        """Moves the unit to the target.  Returns false if and only if
           the unit is out of moves for this turn"""
        success = False
        if self.moves < 1:
            success = False
        else:
            MappableObject.move(self, arg1, arg2)
            self.moves -= 1
            success = True
        return success

    def visibleArea(self):
        """Returns the list of map keys that this unit can see"""
        return MappableObject.listRadius(self.x, self.y, self.sight)
        

class Zombie(Unit):
    """
    Zombies are units that have an associated facing.  Zombies can only
    move to the hex immediately in front of them.  They have the ability
    to smell humans. 
    """
    myType = "Zombie"
    _hp = 70         #see defaults.cfg
    _overheal = 1.50 #see defaults.cfg
    _sight = 4       #see defaults.cfg
    _maxMoves = 1    #see defaults.cfg
    
    _frequency = 0.5       #see defaults.cfg
    _frequencyIncr = 0.01  #see defaults.cfg

    _cap = 20       #see defaults.cfg
    _capIncr = 0.05 #see defaults.cfg

    _unarmedWeaponName = "" #see defaults.cfg
    _weapon = None   #derived from _unarmedWeaponName after loading config file
    
    def __init__(self, game, x, y, facing, owner):
        Unit.__init__(self, game, x, y, Zombie._hp, Zombie._overheal, owner,
                      Zombie._sight, Zombie._maxMoves, Zombie._weapon)
        self.facing = facing
        self.smell = 0

    def toList(self):
        list = Unit.toList(self)
        list.extend([self.facing, self.smell])
        return list

    def nextTurn(self):
        Unit.nextTurn(self)
  
    #isFacing(self, target)
    #isFacing(self, targetX, targetY)
    def isFacing(self, arg1, arg2 = None):
        if arg2 is None:
            targetX = arg1.x
            targetY = arg1.y
        else:
            targetX = arg1
            targetY = arg2 
        inFront = False
        if (self.facing == 0):
            inFront = (targetX > self.x 
                       and targetY > self.x - targetX + self.y)
        elif (self.facing == 1):
            inFront = (targetY > self.x - targetX + self.y
                       and targetY > self.y)
        elif (self.facing == 2):
            inFront = (targetY > self.y and targetX < self.x)
        elif (self.facing == 3):
            inFront = (targetX < self.x
                       and targetY < self.x - targetX + self.y)
        elif (self.facing == 4):
            inFront = (targetY < self.x - targetX + self.y
                       and targetY < self.y)
        elif (self.facing == 5):
            inFront = (targetY < self.y and targetX > self.x)
        return inFront

    def nextX(self):
        """Utility function for determining the X coordinate of 
           the hexagon immediately in front of the zombie."""
        if ((self.facing % 5) == 0): #0 or 5
            retVal = self.x + 1
        elif (((self.facing - 1) % 3) == 0): #1 or 4
            retVal = self.x
        else: #2 or 3
            retVal = self.x - 1
        return retVal

    def nextY(self):
        """Utility function for determining the X coordinate of 
           the hexagon immediately in front of the zombie."""        
        if self.facing >= 4: #4 or 5
            retVal = self.y - 1
        elif (self.facing % 3) == 0: #0 or 3
            retVal = self.y
        else: #1 or 2
            retVal = self.y + 1
        return retVal

    def turn(self, direction):
        """Turn the zombie once.
             direction = 1  : counterclockwise
             direction = -1 : clockwise
             facing = 0     : positive x axis
             facing = 1     : positive y axis 
           Returns true only if the zombie has the moves to do it and
             it is succesful. Otherwise, returns the error"""
        if self.moves < 1:
            return str(self.id) + " can not turn (out of moves)"
        self.moves -= 1
        if (abs(direction) == 1):
            self.facing += direction
        else:
            return str(self.id) + " can not turn (invalid direction)"
        if (self.facing > 5):
            self.facing = 0
        if (self.facing < 0):
            self.facing = 5
        return True

    def eat(self, target):
        """Cannabalism"""
        self.takeDamage(-2*target.hp)
        target.takeDamage(10*target.hp)

    def frequency(turnNum):
        """This value is added to the Zombie player's score at the
           beginning of their turn.  You can spend 1 score to buy 1 zombie.
           Whole score points will be discarded at the end of the zombie's
           turn."""
        return Zombie._frequency + (Zombie._frequencyIncr * turnNum/2)

    def cap(turnNum):
        """This value is the max number of zombies that can be alive
           at any given time."""
        return Zombie._cap + math.floor(turnNum/2 * Zombie._capIncr)

    frequency = staticmethod(frequency)
    cap = staticmethod(cap)


class Human(Unit):
    """
    Humans are units that are capable of grabbing and throwing items and
    weapons.  They specialize in the weapon they start with, causing
    extra damage.
    """
    myType = "Human"
    _config = "config/humans.cfg"
    _overheal = 1.50 #see defaults.cfg
    _sight = 8       #see defaults.cfg
    _maxMoves = 3    #see defaults.cfg
    _hp = 100        #see defaults.cfg
    _ammo = 20       #see defaults.cfg
    _unarmedWeaponName = "" #see defaults.cfg
    _weapon = None   #derived from _unarmedWeaponName after loading config file
    _specialtyBonus = 1.15  #see defaults.cfg

    _cap = 8  #see defaults.cfg

    def __init__(self, game, x, y, weapon, owner):
        self.specialty = weapon
        self.weapon = weapon
        self.sight = Human._sight
        self.overheal = Human._overheal
        self.maxMoves = Human._maxMoves
        self.hp = Human._hp

        #Disable specialties
        #configHuman(self, Human._config)

        Unit.__init__(self, game, x, y, self.hp, self.overheal, owner,
                      self.sight, self.maxMoves, self.specialty)
        self.ammo = Human._ammo

    def nextTurn(self):
        Unit.nextTurn(self)

    def toList(self):
        list = Unit.toList(self)
        list.extend([self.ammo, self.specialty.id])
        return list

    def grab(self, targetCrate):
        success = True
        if (isinstance(targetCrate.contents, WeaponType) and 
          self.weapon == Human._weapon):
            #human is unarmed and picking up a weapon
            self.weapon = targetCrate.contents
        elif targetCrate.contents == Crate._ammoType:
            self.ammo += targetCrate.hp
        else:
            success = False
        return success

    #drop(self)                 : drops the unit's weapon
    #drop(self, itemID)           : drops the unit's weapon only if item is
    #                              the id of the weapon being carried
    #drop(self, itemID, quantity) : drops the unit's ammo only if
    #                              itemID is Crate._ammoType 
    #Returns true if and only if the human is carrying that item
    def drop(self, itemID = None, quantity = 0):
        if (itemID == None or itemID == self.weapon.id):
            self.weapon = Human._weapon
            return True
        elif (itemID == Crate._ammoType and quantity <= self.ammo):
            self.ammo -= quantity
            return True
        return False

    def build(self):
        """Returns true if and only if the human has the moves
           to build a wall object."""
        if self.moves >= 1:
            self.moves -= 1
            return True
        return False
              

class WeaponType(GameObject):
    """
    There will be exactly one instance of this class for each type of
    weapon in the game.
    """
    myType = "WeaponType"

    def __init__(self, game, desc, range, ammo, delay, nonSplDam, vrtSplDam,
                 horSplDam, horSplRad, canSpawnWith, canCall):
        GameObject.__init__(self, game)
        self.desc = desc              #name of the weapon
        self.range = range            #max distance to target
        self.ammo = ammo          #ammo used each firing
        self.delay = delay         #turns between firings, can be noninteger
        self.nonSplashDam = nonSplDam #damage done to target
        self.vrtSplashDam = vrtSplDam #damage done to all in the target hex
        self.horSplashDam = horSplDam #damage done to all in a radius
        self.horSplashRad = horSplRad #radius mentioned on previous line
        self.canSpawnWith = canSpawnWith
        self.canCall = canCall

    def toList(self):
        list = GameObject.toList(self)
        list.extend([self.desc, self.range, self.ammo, self.delay,
                     self.nonSplashDam, self.vrtSplashDam, 
                     self.horSplashDam, self.horSplashRad, self.canSpawnWith,
                     self.canCall])
        return list
    
    def loadConfigList(game):
        list = []
        config = configWeapons('config/weapons.cfg')
        for name in config.keys():
            list.append(WeaponType.fromConfig(game, name, config[name]))
        return list

    def fromConfig(game, name, config):
        new = WeaponType(game,
                         name,
                         config["range"],
                         config["ammo"],
                         config["delay"],
                         config["nonSplashDam"],
                         config["vrtSplashDam"],
                         config["horSplashDam"],
                         config["horSplashRad"],
                         config["canSpawnWith"],
                         config["canCall"])
        return new

    loadConfigList = staticmethod(loadConfigList)
    fromConfig = staticmethod(fromConfig)
        

class Game:
    """
    The Game class handles all of the interactions between the different
    objects.  The game keeps lists of each object by type.  It keeps
    a dictionary to allow for looking up objects by their id.  Finally,
    it also keeps a dictionary which turns a map key (see 
    MappableObject.mapKey) into a list of objects at that location.

    Note to Future Semester Programmer:
    In future versions of Game.py, less should take place in this class.
    Keep with the object oriented approach and put more of the action
    in the other classes, utilizing the inheritance structure as much as
    possible.

    For example: You could have a GameObject.attack() function that simply
      returns str(self.id) + " can not attack".  Then you could specify an
      attack function in the unit class.  Then when they request that A
      attacks B, you just make sure A and B exists and then call A.attack(B).
    """
    _maxTurns = 500 #see defaults.cfg
    _clockStart = 180 #see defaults.cfg
    _secondsPerTurn = 2 #see defaults.cfg

    def __init__(self, id):
        self.nextid = 0
        self.maxid = 2147483600
        self.id = int(id)
        self.players = [] #list of player objects
        self.humanPlayer = None
        self.zombiePlayer = None
        self.turn = None #the player whose turn it is; None before and after the game.
        self.turnNum = 0
        self.winner = None
        random.seed()
        self.objects = dict() #key: object's id
                              #value: instance of the object
        self.map = dict()     #key: string "x, y"
                              #value: list of objects
        self.humans = []      
        self.zombies = []     
        self.walls = []       
        self.airstrikes = []  
        self.crates = []      
        self.weapons = []
        self.spawns = []

        self.animations = []
        self.deadBabies = []

        self.loadConfigFiles()
        self.maxTurns = int(Game._maxTurns)
        self.humanTime = int(Game._clockStart)
        self.zombieTime = int(Game._clockStart)

    def loadConfigFiles(self):
        weaponList =  WeaponType.loadConfigList(self)
        for w in weaponList:
            self.addObject(w)
        configClassDefaults(globals(), CONFIG) 
  
        for w in weaponList:
            if str(w.desc) == str(Zombie._unarmedWeaponName):
                Zombie._weapon = w
            if w.desc == Human._unarmedWeaponName:
                Human._weapon = w

    def start(self):
        if self.humanPlayer is None or self.zombiePlayer is None or self.turn or self.winner:
            return False
        
        self.players.insert(0, self.humanPlayer)
        self.players.insert(1, self.zombiePlayer)
        self.log = game_log((self.humanPlayer.user, self.zombiePlayer.user))
        for i in self.players:
            i.score = 0
            i.hasMap = None
        self.turnNum = 0
        self.airstrikesReady = 0
        self.zombiesReady = 0
        self.sendIdent(self.players)
        list = []
        self.loadMap()
        self.nextTurn()
        return True

    def loadMap(self):
        #TODO : Actually load a map created by a map editor.
        files = dircache.listdir("./maps")
        for f in files:
            if not f.endswith(".map"):
                files.remove(f)
        index = random.randint(0,len(files)-1)
        inFile = open("./maps/" + files[index], "r")
        b = MappableObject._boundaryRadius
        y = -b
        x = -b
        self.addObject(SpawnZone(self, -b, -b, -b,  b))
        self.addObject(SpawnZone(self, -b,  b,  b,  b))
        self.addObject(SpawnZone(self,  b,  b,  b, -b))
        self.addObject(SpawnZone(self,  b, -b, -b, -b))
        input = inFile.readlines()
        for line in input:
            x = -b
            while (len(line) > 0):
                if line[0] == "W":
                    self.addObject(Wall(self, x, y, 600))
                elif line[0] == "w":
                    self.addObject(Wall(self, x, y, 200))
                elif line[0] == "C":
                    self.addObject(Crate(self, x, y, Crate._ammoType, 40))
                x += 1
                line = line[1:]
            y += 1
        inFile.close()
        

        """
        b = MappableObject._boundaryRadius
        self.addObject(SpawnZone(self, -b, -b, -b,  b))
        self.addObject(SpawnZone(self, -b,  b,  b,  b))
        self.addObject(SpawnZone(self,  b,  b,  b, -b))
        self.addObject(SpawnZone(self,  b, -b, -b, -b))
        for x in range(-b+1, b-1):
            for y in range(-b+1, b-1):
                xMod = abs(x)%7
                yMod = abs(y)%7
                if (yMod == 0 and (xMod == 3 or xMod == 4)):
                    self.addObject(Wall(self, x, y, 300))
                if (xMod == 0 and (yMod == 2 or yMod == 3 or yMod == 4)):
                    self.addObject(Wall(self, x, y, 300))
                if (xMod == 3 and yMod == 3):
                    self.addObject(Crate(self, x, y, Crate._ammoType, 15))            
         """


    #Determines whether or not this game is over.
    #If the game is over, inform both players, return true
    #Otherwise, return false
    def declareWinner(self):
        if (self.zombieTime <= 0):
            self.turnNum = int(self.maxTurns)
        if (self.humanTime <= 0):
            for hum in self.humans:
                hum.hp = 0
                self.checkHP(hum)
        if (self.turnNum >= self.maxTurns):
            self.winner = self.humanPlayer
        if (len(self.humans) == 0 and self.turnNum >= 1):
            self.winner = self.zombiePlayer
        if self.winner:
            self.log.declare_winner(self.winner.user)
            for i in self.players:
                i.writeSExpr(['game-over', self.winner.ID, self.turnNum])
                i.hasMap = None
            self.sendStatus(self.players)
            print "Game " + `self.id` + " ended!"
            print "    Winner:" + str(self.winner.user)
            print "    Turns :" + str(self.turnNum)
            file('records.dat', 'a').write('%s %s %s %s %s\n' % \
                      (self.humanPlayer.user, self.zombiePlayer.user, \
                       self.turnNum, self.humanTime, self.zombieTime))
            for i in self.players:
                if(i.type != "spectator"):
                  i.disconnect()
            return True 
        return False #No winner
   
    def nextTurn(self):
        if (not self.turn and self.turnNum > 1) or self.winner:
            return False;

        if (self.turnNum >1):
            if (self.turn == self.zombiePlayer):
                self.zombieTime += (self.turnStart - time.time())
                self.zombieTime += Game._secondsPerTurn
            else:
                self.humanTime += (self.turnStart - time.time())
                self.humanTime += Game._secondsPerTurn

        if (self.declareWinner()):
            return True

        for nextObject in self.objects.values():
            nextObject.nextTurn()
        for nextStrike in self.airstrikes:
            if nextStrike.ready():
                self.executeAirstrike(nextStrike)
        for nextZombie in self.zombies:
            self.setSmell(nextZombie)
        
        self.turnNum += 1
        
        if self.turnNum == 1 or self.turn == self.zombiePlayer:
            #Human turn
            self.turn = self.humanPlayer
            self.airstrikesReady += Airstrike._frequency
            self.airstrikesReady = min(self.airstrikesReady, 1)
            self.zombiesReady = self.zombiesReady \
                                - math.floor(self.zombiesReady)
        elif self.turn == self.humanPlayer:
            #zombie turn
            self.zombiesReady += Zombie.frequency(self.turnNum)
            self.turn = self.zombiePlayer
        else:
            self.turn = self.humanPlayer

        if self.turnNum == 1:
            humansReady = Human._cap - len(self.humans)
        else:
            humansReady = 0

        self.sendStatus(self.players)
        self.writeToLog()
        self.turnStart = time.time()
        self.animations = [] #events that happened this turn
        self.deadBabies = [] #units that died on the same turn they were born

    def writeToLog(self):
        message = ""
        if self.turnNum == 1:
            humansReady = Human._cap - len(self.humans)
            identList = []
            identMessage = ""
            for i in self.players:
                identList += [[i.ID, i.user, i.screenName, i.type]]
            identList = ['ident', identList, self.log.id, 
                         MappableObject._boundaryRadius]
            identMessage = sexpr2str(identList) + '\n'
            self.log.append_status(identMessage)
        else:
            humansReady = 0
        list = ["status"]
        list.append(["Game", self.turnNum, humansReady,
                   self.airstrikesReady, self.zombiesReady,
                   Zombie.cap(self.turnNum), self.humanTime, self.zombieTime])
        if (self.turnNum == 1):
            list.append(['Weapons'] + [j.toList() for j in self.weapons])
        if (len(self.walls) > 0):
            list.append(['Walls'] + [j.toList() for j in self.walls])
        if (len(self.spawns) > 0 and self.turnNum == 1):
            list.append(['Spawns'] + [j.toList() for j in self.spawns])
        if (len(self.crates)):
            list.append(['Crates'] + [j.toList() for j in self.crates])
        if (len(self.humans)>0):
            list.append(['Humans'] + [j.toList() for j in self.humans])
        if (len(self.zombies)>0):
            list.append(['Zombies'] + [j.toList() for j in self.zombies])
        if (len(self.airstrikes) > 0):
            list.append(['Airstrikes'] + [j.toList() for j in self.airstrikes])
        if (len(self.animations)>0):
            list.append(['Animations'] + [j for j in self.animations])
        if (len(self.deadBabies)>0):
            list.append(['Dead'] + self.deadBabies)

        
        
        #write to file
        #filename = "logs/" + str(self.id).zfill(5) + "." \
                   #+ str(self.humanPlayer.user) + "." \
                   #+ str(self.zombiePlayer.user) + ".gamelog"
        #if (self.turnNum == 1):
            #file = open(filename,"w")
        #else:
            #file = open(filename,"a")
        #file.write(sexpr2str(message))
        #file.close       

        #write to db
        self.log.append_status(sexpr2str(list)+'\n')

    def sendStatus(self, players):
        for i in players:
            list = ["status"]
            if self.turnNum == 1:
                humansReady = Human._cap - len(self.humans)
            else:
                humansReady = 0
            list.append(["Game", self.turnNum, humansReady,
                   self.airstrikesReady, self.zombiesReady,
                   Zombie.cap(self.turnNum), self.humanTime, self.zombieTime])
            if not i.hasMap:
                self.sendIdent([i])
                list.append(['Weapons'] + [j.toList() for j in self.weapons])
                list.append(['Walls'] + [j.toList() for j in self.walls])
                list.append(['Spawns'] + [j.toList() for j in self.spawns])
                if (not i.type == "zombie"):
                    list.append(['Crates'] + [j.toList() for j in self.crates])
                i.hasMap = True
            list.extend(self.statusList(i))
            if(i.type == "spectator"):
                if len(self.animations) != 0:
                    list.append(['Animations'] + [j for j in self.animations])
                if len(self.deadBabies) != 0:
                    list.append(['Dead'] + self.deadBabies)
            i.writeSExpr(list)

    def statusList(self, player):
        """Returns the status list of all things visible to
        the given player."""
        list = []
        
        walls = []
        zombies = []
        humans = []
        airstrikes = []
        crates = []
        
        for hexKey in self.visibleArea(player):
            for obj in self.map.get(hexKey, []):
                if isinstance(obj, Crate):
                    crates.append(obj.toList())
                elif isinstance(obj, Wall):
                    walls.append(obj.toList())
                elif isinstance(obj, Zombie):
                    zombies.append(obj.toList())
                elif isinstance(obj, Human):
                    humans.append(obj.toList())
                elif isinstance(obj, Airstrike):
                    airstrikes.append(obj.toList())
               
        if len(walls) != 0:
            list.append(['Walls'] + walls)
        if len(humans) != 0:
            list.append(['Humans'] + humans)
        if len(zombies) != 0:
            list.append(['Zombies'] + zombies)
        if player.type != "zombie" and len(airstrikes) != 0:
            list.append(['Airstrikes'] + airstrikes)
        
        if player.type != "zombie" and len(crates) != 0:
            list.append(['Crates'] + crates)
            
        return list

    def sendIdent(self, players):
        if len(self.players) < 2:
            return False #bad bad bad
        list = []
        for i in self.players:
            list += [[i.ID, i.user, i.screenName, i.type]]
        for i in players:
            i.writeSExpr(['ident', list, self.log.id, 
                  MappableObject._boundaryRadius])
    
    def addPlayer(self, player):
        allowed = False
        if player.type in ['human']:
            if self.humanPlayer is not None:
                player.writeSExpr(['join-denied', 'already have human player'])
            else:
                self.humanPlayer = player
                allowed = True
            
        elif player.type in ['zombie']:
            if self.zombiePlayer is not None:
                player.writeSExpr(['join-denied', 'already have zombie player'])
            else:
                self.zombiePlayer = player
                allowed = True
                
        elif player.type in ['spectator']:
            self.players.append(player)
            allowed = True
        
        else:
            self.humanPlayer = player
            player.type = "human"
            allowed = True
        
        return allowed

    def removePlayer(self, player):
        try:
            if player.type == 'human':
                self.humanPlayer = None
            elif player.type == 'zombie':
                self.zombiePlayer = None
            else:
                self.players.remove(player) #spectators
            
            if self.turn:
                self.players.remove(player) #human/zombie in game WHILE game in progress
                self.turn = None
                for i in self.players:
                    if not i.type == 'spectator':
                        self.winner = i #the remaining nonspectating player
                for i in self.players:
                    i.writeSExpr(['game-over', self.id])
                print "Game " + `self.id` + " ended! Winner:" + self.winner.user
            return True
        except:
            return False
            
    def addSpectator(self, spectator):
        self.spectators.append(spectator)
        return True
        
    def removeSpectator(self, spectator):
        try:
            self.spectators.remove(spectator)
            return True
        except:
            return False
     
    def listPlayers(self):
        list = []
        for i in self.players:
            list.append([i.user, i.type])
        
        if(self.winner is None and self.turn is None):
            if self.humanPlayer is not None:
                list.insert(0, [self.humanPlayer.user, self.humanPlayer.type])
            if self.zombiePlayer is not None:
                list.insert(1, [self.zombiePlayer.user, self.zombiePlayer.type])
 
        return list
     
    def addObject(self, newObject):
        self.animations += [["add", newObject.id]]
        self.objects[newObject.id] = newObject
        if (isinstance(newObject, WeaponType)):
            self.weapons.append(newObject)
        if (isinstance(newObject, Human)):
            self.humans.append(newObject)
        if (isinstance(newObject, Zombie)):
            self.zombies.append(newObject)
        if (isinstance(newObject, Airstrike)):
            self.airstrikes.append(newObject)
        if (isinstance(newObject, Wall)):
            self.walls.append(newObject)
        if (isinstance(newObject, Crate)):
            self.crates.append(newObject)
        if (isinstance(newObject, SpawnZone)):
            self.spawns.append(newObject)

        if (isinstance(newObject, MappableObject)):
            self.addToMap(newObject)

    def removeObject(self, oldObject):
        self.animations += [["remove", oldObject.id]]
        if oldObject.turnCreated == self.turnNum:
            self.deadBabies += [oldObject.toList()]
        if (isinstance(oldObject, WeaponType)):
            self.weapons.remove(oldObject)
        if (isinstance(oldObject, Human)):
            self.humans.remove(oldObject)
        if (isinstance(oldObject, Zombie)):
            self.zombies.remove(oldObject)
        if (isinstance(oldObject, Airstrike)):
            self.airstrikes.remove(oldObject)
        if (isinstance(oldObject, Wall)):
            self.walls.remove(oldObject)
        if (isinstance(oldObject, Crate)):
            self.crates.remove(oldObject)
        if (isinstance(oldObject, SpawnZone)):
            self.spawns.remove(oldObject)

        del self.objects[oldObject.id]
        if (isinstance(oldObject, MappableObject)):
            self.removeFromMap(oldObject)
      
    def addToMap(self, newObject):
        key = newObject.mapKey()
        if self.map.has_key(key):
            self.map[key].append(newObject)
        else:
            self.map[key] = [newObject]

    def removeFromMap(self, oldObject):
        key = oldObject.mapKey()
        if self.map.has_key(key):
            self.map[key].remove(oldObject)
            if (len(self.map[key]) == 0):
                del self.map[key]

    def visibleArea(self, player):
        if player.type == "human": # #Human player
            myUnits = self.humans
        elif player.type == "zombie": #zombie player
            myUnits = self.zombies
        else:
            myUnits = []
            #myUnits.extend(self.humans)
            #myUnits.extend(self.zombies)
        visArea = dict() #keys are the hex areas that can be
                         #   seen by this player
                         #values are arbitrary
        for u in myUnits:
            for hexKey in u.visibleArea():
                visArea[hexKey] = True
        if player.type == "spectator":
            b = MappableObject._boundaryRadius
            for x in range(-b-1, b+1):
                for y in range(-b-1, b+1):
                    visArea[MappableObject.toMapKey(x,y)] = True
        hexKeyList = visArea.keys()
        visArea.clear()
        return hexKeyList

    #move(self, zombieID)                 : moves a zombie forward
    #move(self, unitID, targetX, targetY) : moves a unit to the target
    def move(self, unitID, targetX = None, targetY = None):
        myUnit = self.objects.get(unitID)
        if myUnit is None:
            return str(unitID) + " can not move (does not exist)"
        if not isinstance(myUnit, Unit):
            return str(unitID) + " can not move (is not a unit)"
        if not myUnit.owner == self.turn:
            return str(unitID) + " can not move (not your unit)"
        if isinstance(myUnit, Zombie):
            if targetX is None:
                targetX = myUnit.nextX
            if targetY is None:
                targetY = myUnit.nextY
            if not myUnit.isFacing(targetX, targetY):
                return str(unitID) + " can not move there (not facing target)"
            targetKey = MappableObject.toMapKey(targetX, targetY)
            for obj in self.map.get(targetKey,  []):
                if isinstance(obj, Wall):
                    return str(unitID)+" can not move there (Wall in the way)"
                elif isinstance(obj, Human):
                    return str(unitID)+" can not move there (Human in the way)"
        else: #Unit is a human
            if targetX is None or targetY is None:
                return str(unitID) + "can not move there (no target specified)"
            targetKey = MappableObject.toMapKey(targetX, targetY)
            for obj in self.map.get(targetKey, []):
                if isinstance(obj, Zombie):
                    return str(unitID)+" can not move there (Zombie in the way)"
                if isinstance(obj, Wall):
                    return str(unitID)+" can not move there (Wall in the way)"
                if isinstance(obj, Human):
                    return str(unitID)+" can not move there (Human in the way)"
        if not myUnit.distanceTo(targetX, targetY) == 1:
            return str(unitID)+" can not move there (target must be adjacent)"
        if not MappableObject.inBounds(targetX, targetY):
            return str(unitID) + " can not move there (target out of bounds)"
        self.removeFromMap(myUnit)
        if not (myUnit.move(targetX, targetY)):
            self.addToMap(myUnit) #Return the unit from whence it came
            return str(unitID) + " can not move (out of moves)"
        self.addToMap(myUnit)
        self.animations += [["move", myUnit.id, targetX, targetY]]
        return True

    #def attack(self, attackerID, targetX, targetY):
    #def attack(self, attackerID, targetID):
    def attack(self, attackerID, arg1, arg2 = None):
        inVrtSplash = [] #object ids in vertical splash
        inHorSplash = [] #object ids in horizontal splash
        targetID = None  #object id of direct target
        targetX = None
        targetY = None
        target = None
        multiplier = 1
        if (arg2 == None):
            targetID = arg1
            target = self.objects.get(targetID, None)
            if (target == None):
                return str(targetID) + " can not be attacked (does not exist)"
            if (not isinstance(target, HittableObject)):
                return str(targetID) + " can not be attacked (not targetable)"
            targetX = target.x
            targetY = target.y
        else:
            targetX = arg1
            targetY = arg2
        attacker = self.objects.get(attackerID, None)
        if (attacker == None):
            return str(attackerID) + " can not attack (does not exist)"
        if (not isinstance(attacker, Unit)):
            return str(attackerID) + " can not attack (not a unit)"
        if not attacker.owner == self.turn:
            return str(attackerID) + " can not attack (not your unit)"
        weapon = attacker.weapon
        if (weapon == None):
            return str(attackerID) + " can not attack (has no weapon)"
        if (attacker.distanceTo(targetX, targetY) > int(weapon.range)):
            #print attacker.distanceTo(targetX, targetY)
            #print weapon.range
            return str(attackerID) + " can not attack (target out of range)" \
                         + str(attacker.distanceTo(targetX, targetY)) + " " \
                         + str(weapon.range)
        if (attacker.ammo < weapon.ammo):
            return str(attackerID) + " can not attack (not enough ammo)"
        if (isinstance(attacker, Zombie) and isinstance(target, Crate)):
            return str(attackerID) + " can not attack (can not see that)"
        if not attacker.useTime(weapon.delay):
            return str(attackerID) + " can not attack (weapon not ready)"
        if isinstance(attacker, Zombie):
            if (attacker.moves < 1):
                return str(attackerID) + " can not attack (out of moves)"
            attacker.moves -= 1
        attacker.ammo -= weapon.ammo
        if isinstance(attacker, Human):
            if attacker.specialty.id == attacker.weapon.id:
                multiplier = Human._specialtyBonus
        self.animations += [["attack", attacker.id, targetX, targetY]]

        #Generate and append hurt animation
        hurtUnits = [] #All the units that were hit with this attack
        hurtUnits += self.splashDamage(targetX, targetY, weapon.horSplashRad, 0)
        hurtAni = []
        for h in hurtUnits:
            hurtAni += [h.id]
        hurtAni = ["hurt"] + [hurtAni]
        self.animations += [hurtAni]

        #Targeted Damage
        if (not target is None):
            target.takeDamage(weapon.nonSplashDam * multiplier)
            self.checkHP(target)
        #Horizontal Splash
        self.splashDamage(targetX, targetY, weapon.horSplashRad, 
                          weapon.horSplashDam * multiplier)
        #Vertical Splash
        self.splashDamage(targetX, targetY, 0, 
                          weapon.vrtSplashDam * multiplier)
        return True

    #Inflicts splash damage
    #Returns the list of units hit by this attack
    def splashDamage(self, targetX, targetY, radius, damage):
        hitList = []
        for hex in MappableObject.listRadius(targetX, targetY, radius):
            targets = list(self.map.get(hex, []))
            hitList += targets
            for splashTarget in targets:
                if isinstance(splashTarget, HittableObject):
                    splashTarget.takeDamage(damage)
                    self.checkHP(splashTarget)
        return hitList
             
    def checkHP(self, target):
        newCrate1 = None
        newCrate2 = None
        if (target.isDestroyed()):
            if isinstance(target,Human):
                if (target.ammo > 0):
                    newCrate1 = Crate(self, target.x, target.y, \
                                      Crate._ammoType, target.ammo)
                    newCrate2 = Crate(self, target.x, target.y, target.weapon)  
            self.removeObject(target)
            if (not newCrate1 is None):
                self.addObject(newCrate1)
            if (not newCrate2 is None):
                self.addObject(newCrate2)

    def eat(self, zombieID, targetID):
        myZombie = self.objects.get(zombieID) 
        if myZombie is None:
            return str(zombieID) + " can not eat (does not exist)"
        if not isinstance(myZombie, Zombie):
            return str(zombieID) + " can not eat (is not a zombie)"
        if not myZombie.owner == self.turn:
            return str(zombieID) + " can not eat (not your unit)"
        target = self.objects.get(targetID)
        if target is None:
            return str(targetID) + " can not be eaten (is not a zombie)"
        if not isinstance(target, Zombie):
            return str(targetID) + " can not be eaten (is not a zombie)"
        if myZombie.distanceTo(target) > 1:
            return str(zombieID) + " can not eat " + str(targetID) +\
                   " (target is out of reach)"
        if (not myZombie.isFacing(target)) \
            and myZombie.distanceTo(target) > 0:
            return str(zombieID) + " can not eat " + str(targetID) +\
                   " (must be facing target or in the same hex)"
        self.animations += [["eat", myZombie.id, target.id]]
        myZombie.eat(target)
        myZombie.moves -= 1
        self.checkHP(myZombie)
        self.checkHP(target)
        return True

    def rotate(self, zombieID, direction):
        """Rotates a zombie"""
        errBuff = False
        myZombie = self.objects.get(zombieID)
        if myZombie is None:
            return str(zombieID) + " can not turn (does not exist)"
        if not isinstance(myZombie, Zombie):
            return str(zombieID) + " can not turn (is not a zombie)"
        if not myZombie.owner == self.turn:
            return str(zombieID) + " can not turn (not your unit)"
        errBuff = myZombie.turn(direction)
        if errBuff != True:
            return errBuff;
        self.animations += [["turn", zombieID, direction]]
        return True;

    def executeAirstrike(self, strike):
        """Drops the crate, causing damage to anything in the way."""
        self.removeObject(strike)
        self.splashDamage(strike.x, strike.y, 0, Airstrike._damage)
        newCrate = Crate(self, strike.x, strike.y, strike.contents)
        if newCrate.isInBounds():
            self.addObject(newCrate)

    def callAirstrike(self, x, y, contents):
        """Places an airdrop object on the map.  After a few turns, the 
           airdrop creates a crate and disappears."""
        #contents is an integer, -1, -2, or a weapon type's id.
        if not self.turn == self.humanPlayer:
            return "can not call airstrike (only the human player can do that)"
        if self.airstrikesReady < 1:
            return "can not call airstrike (no airstrikes ready)"
        if (contents == Crate._ammoType):
            self.addObject(Airstrike(self, x, y, contents))
        elif isinstance(self.objects.get(contents), WeaponType):
            if (not self.objects.get(contents).canCall):
                return "can not call airstrike (invalid weapon)"
            self.addObject(Airstrike(self, x, y, self.objects.get(contents)))
            
        self.airstrikesReady -= 1
        return True

    def setSmell(self, zombie):
        zombie.smell = 0
        for nextHuman in self.humans:
            #print str(nextHuman.id) + "?"
            if zombie.isFacing(nextHuman):
                #print str(1.0 / zombie.distanceTo(nextHuman))
                zombie.smell += 1.0 / zombie.distanceTo(nextHuman)
    
    def grab(self, unitID, crateID):
        myUnit = self.objects.get(unitID)
        if myUnit is None:
            return str(unitID) + " can not grab (does not exist)"
        if not isinstance(myUnit, Human):
            return str(unitID) + " can not grab (is not human)"
        if not myUnit.owner == self.turn:
            return str(unitID) + " can not grab (not your unit)"
        myCrate = self.objects.get(crateID)
        if myCrate is None:
            return str(unitID) + " can not be grabbed (does not exist)"
        if not isinstance(myCrate, Crate):
            return str(crateID) + " can not be grabbed (is not a crate)"
        if myUnit.distanceTo(myCrate) > 1:
            return str(crateID)+" can not be grabbed (out of reach)"
        if not myUnit.grab(myCrate): #attempt to grab
            return str(unitID) + " can not grab (can not carry contents)"
        self.animations += [["grab", unitID, crateID]]
        myCrate.hp = 0
        self.checkHP(myCrate)
        return True

    def throw(self, unitID, itemID, targetX = None, 
              targetY = None, quantity = 0):
        if not MappableObject.inBounds(targetX, targetY):
            return str(unitID) + " can not throw (target out of bounds)"
        if not self.objects.has_key(unitID):
            return str(unitID) + " can not throw (does not exist)"
        myUnit = self.objects[unitID]
        if not isinstance(myUnit, Human):
            return str(unitID) + " can not throw (not a human)"
        if not myUnit.owner == self.turn:
            return str(unitID) + " can not throw (not your unit)"
        if targetX is None:
            targetX = myUnit.x
        if targetY is None:
            targetY = myUnit.y
        carriedWeapon = myUnit.weapon
        if myUnit.distanceTo(targetX, targetY) > 1:
            return str(unitID)+" can not throw there (too far away)"
        if carriedWeapon == Human._weapon:
            return str(unitID)+" can not throw " \
                   + "(Emotionally Attached to Chainsaw."
        if not myUnit.drop(itemID, quantity):
            return str(unitID) + " can not throw (is not carrying that)"
        self.animations += [["throw", unitID, itemID, targetX, targetY]]
        if itemID == Crate._ammoType:
            newCrate = Crate(self, targetX, targetY, itemID, quantity)
        else:
            newCrate = Crate(self, targetX, targetY, carriedWeapon, 100)
        if not (newCrate is None):
            newCrate.x = targetX
            newCrate.y = targetY
            self.addObject(newCrate)
        return True

    def giveAmmo(self, giverID, takerID, quantity):
        if quantity <= 0:
            return "can not give negative ammo."
        myGiver = self.objects.get(giverID)
        if myGiver is None:
            return str(giverID) + " can not give (does not exist)"
        if not isinstance(myGiver, Human):
            return str(giverID) + " can not give (is not a human)"
        if not myGiver.owner == self.turn:
            return str(humanID) + " can not give (not your unit)"
        if quantity > myGiver.ammo:
            return str(giverID) + " can not give (not enough ammo)"
        myTaker = self.objects.get(takerID)
        if myTaker is None:
            return str(takerID) + " can not recieve (does not exist)"
        if not isinstance(myTaker, Human):
            return str(takerID) + " can not recieve (is not a human)"
        if (myGiver.distanceTo(myTaker) > 1):
            return str(myGiver) + " can not give (target out of range)"
        myGiver.ammo -= quantity
        myTaker.ammo += quantity
        self.animations += [["give", giverID, takerID]]
        return True
        
    def buildWall(self, humanID, targetX, targetY):
        myHuman = self.objects.get(humanID)
        if not MappableObject.inBounds(targetX, targetY):
            return str(humanID) + " can not build (target out of bounds)"
        if myHuman is None:
            return str(humanID) + " can not build (does not exist)"
        if not isinstance(myHuman, Human):
            return str(humanID) + " can not build (is not a human)"
        if not myHuman.owner == self.turn:
            return str(humanID) + " can not build (not your unit)"
        if not myHuman.distanceTo(targetX, targetY) == 1:
            return str(humanID)+" can not build (target must be adjacent)"
        targetWall = None
        targetKey = MappableObject.toMapKey(targetX, targetY)
        for obj in self.map.get(targetKey, []):
            if isinstance(obj, Human):
                return str(humanID) + " can not build (Human in the way)"
            if isinstance(obj, Zombie):
                return str(humanID) + " can not build (Zombie in the way)"
            if isinstance(obj, Wall):
                targetWall = obj
        if not myHuman.build():
            return str(humanID) + " can not build (no moves left)"
        self.animations += [["build", humanID, targetX, targetY]]
        if targetWall is None:
            self.addObject(Wall(self, targetX, targetY, Wall._hpIncr))
        else:
            targetWall.takeDamage(-Wall._hpIncr)
        return True

    def spawnZombie(self, spawnID, facing):
        """Create a zombie at the target location"""
        if not self.turn == self.zombiePlayer:
            return "Zombie can not spawn (only the zombie player can do that)"
        if len(self.zombies) >= Zombie.cap(self.turnNum):
            return "Zombie can not spawn (Too many zombies)"
        if self.zombiesReady < 1:
            return "Zombie can not spawn (No zombies are ready)"
        if facing < 0 or facing > 5:
            return "Zombie can not spawn (invalid facing)"
        myZone = self.objects.get(spawnID, None)
        if (myZone is None):
            return "Zombie can not spawn (invalid spawn zone ID)"
        if not isinstance(myZone, SpawnZone):
            return "Zombie can not spawn (invalid spawn zone ID)"
        spawnHex = myZone.pop()
        while not MappableObject.inBounds(spawnHex[0], spawnHex[1]):
            spawnHex = myZone.pop()
        #HEX MAP
        #if not (MappableObject.distance(targetX, targetY, 0, 0) \
        #        == MappableObject._boundaryRadius):
        #    return "Zombie can not spawn (must target a hex on the edge" + \
        #           " of the map)"
        self.addObject(Zombie(self, spawnHex[0], spawnHex[1], facing,\
                              self.zombiePlayer))
        self.zombiesReady -= 1
        return True

    def spawnHuman(self, targetX, targetY, weaponID):
        if self.turnNum > 1:
            return "Human can not spawn (Must be turn 1)"
        if not MappableObject.inBounds(targetX, targetY):
            return "Human can not spawn (target out of bounds)"
        if len(self.humans) >= Human._cap:
            return "Human can not spawn (Too many humans)"
        if not self.objects.has_key(weaponID):
            return "Human can not spawn (object " + str(weaponID) + \
                   " does not exist)"
        myWeapon = self.objects.get(weaponID)
        if not isinstance(myWeapon, WeaponType):
            return "Human can not spawn (object " + str(weaponID) + \
                   " is not a weapon)"
        if (not myWeapon.canSpawnWith):
            return "Human can not spawn (object " + str(weaponID) + \
                   " is an invalid weapon)"
        targetKey = MappableObject.toMapKey(targetX, targetY)
        for obj in self.map.get(targetKey, []): 
            if isinstance(obj, Human):
                return "Human can not spawn (Human in the way)"
            if isinstance(obj, Wall):
                return "Human can not spawn (Wall in the way)"

        self.addObject(Human(self, targetX, targetY, myWeapon, \
                             self.humanPlayer))
        return True

    def chat(self, player, message):
        for i in self.players:
            i.writeSExpr(['says', player.user, message])
        return True


if __name__ == "__main__":
    myGame = Game(0)   
    print "Zombie Default HP = " + str(Zombie._hp)
    print "Game Max Turns = " + str(Game._maxTurns)

