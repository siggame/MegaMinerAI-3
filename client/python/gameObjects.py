from library import library

from ExistentialError import ExistentialError

class GameObject(object):
  def __init__(self, ptr):
    from BaseAI import BaseAI
    self.ptr = ptr
    self.iteration = BaseAI.iteration
    #the following is a bit of metaprogramming
    #inheriting classes probably want to override all of this
    #self.n is this class' name
    c = `self.__class__`
    self.n = c[c.rfind('.')+1:c.rfind("'")]
    #guess the function to get the ID for this class
    self.ID = library.__getattribute__(self.n.lower() + 'GetID')(ptr)
  
  def validify(self):
    from BaseAI import BaseAI
    #if this class is pointing to an object from before the current turn it's probably
    #somewhere else in memory now
    if self.iteration == BaseAI.iteration:
      return True
    #guess where BaseAI stores these objects
    for i in BaseAI.__getattribute__(self.n.lower() + 's'):
      if i.ID == self.ID:
        self.ptr = i.ptr
        self.iteration = BaseAI.iteration
        return True
    raise ExistentialError()

##Airstrike
class Airstrike(GameObject):
  ##Unique ID
  def getID(self):
    self.validify()
    return library.airstrikeGetID(self.ptr)
  ##X location
  def getX(self):
    self.validify()
    return library.airstrikeGetX(self.ptr)
  ##Y location
  def getY(self):
    self.validify()
    return library.airstrikeGetY(self.ptr)
  ##Turns until landing
  def getDelay(self):
    self.validify()
    return library.airstrikeGetDelay(self.ptr)
  ##Weapon being dropped (ammo == -1)
  def getItemID(self):
    self.validify()
    return library.airstrikeGetItemID(self.ptr)

##Crate
class Crate(GameObject):
  ##Unique ID number
  def getID(self):
    self.validify()
    return library.crateGetID(self.ptr)
  ##X coordinate
  def getX(self):
    self.validify()
    return library.crateGetX(self.ptr)
  ##Y coordinate
  def getY(self):
    self.validify()
    return library.crateGetY(self.ptr)
  ##Current HP
  def getHp(self):
    self.validify()
    return library.crateGetHp(self.ptr)
  ##Maximum HP
  def getMaxHp(self):
    self.validify()
    return library.crateGetMaxHp(self.ptr)
  ##Contents of the crate; Weapon ID or -1 for ammo
  def getContent(self):
    self.validify()
    return library.crateGetContent(self.ptr)

##Human
class Human(GameObject):
  ##Move to the adjacent hex at x y
  def move(self, x, y):
    self.validify()
    if x.__class__ not in [int, float]:
      raise TypeError('x should be of [int, float]')
    if y.__class__ not in [int, float]:
      raise TypeError('y should be of [int, float]')
    return library.humanMove(self.ptr, x, y)
  ##Attack the Zombie
  def attack(self, target):
    self.validify()
    if target.__class__ not in [Zombie]:
      raise TypeError('target should be of [Zombie]')
    target.validify()
    return library.humanAttack(self.ptr, target.ptr)
  ##Attack the Crate
  def attackCrate(self, target):
    self.validify()
    if target.__class__ not in [Crate]:
      raise TypeError('target should be of [Crate]')
    target.validify()
    return library.humanAttackCrate(self.ptr, target.ptr)
  ##Attack the Wall
  def attackWall(self, target):
    self.validify()
    if target.__class__ not in [Wall]:
      raise TypeError('target should be of [Wall]')
    target.validify()
    return library.humanAttackWall(self.ptr, target.ptr)
  ##Attack hex at X,Y
  def attackGround(self, x, y):
    self.validify()
    if x.__class__ not in [int, float]:
      raise TypeError('x should be of [int, float]')
    if y.__class__ not in [int, float]:
      raise TypeError('y should be of [int, float]')
    return library.humanAttackGround(self.ptr, x, y)
  ##Take the contents of a crate in the same hex
  def grab(self, target):
    self.validify()
    if target.__class__ not in [Crate]:
      raise TypeError('target should be of [Crate]')
    target.validify()
    return library.humanGrab(self.ptr, target.ptr)
  ##Give ammo to an adjacent friend
  def give(self, target, quantity):
    self.validify()
    if target.__class__ not in [Human]:
      raise TypeError('target should be of [Human]')
    target.validify()
    if quantity.__class__ not in [int, float]:
      raise TypeError('quantity should be of [int, float]')
    return library.humanGive(self.ptr, target.ptr, quantity)
  ##Drops equipped weapon in an adjacent hex
  def throwWeapon(self, x, y):
    self.validify()
    if x.__class__ not in [int, float]:
      raise TypeError('x should be of [int, float]')
    if y.__class__ not in [int, float]:
      raise TypeError('y should be of [int, float]')
    return library.humanThrowWeapon(self.ptr, x, y)
  ##Drops an amount of ammo in an adjacent hex
  def throwAmmo(self, x, y, quantity):
    self.validify()
    if x.__class__ not in [int, float]:
      raise TypeError('x should be of [int, float]')
    if y.__class__ not in [int, float]:
      raise TypeError('y should be of [int, float]')
    if quantity.__class__ not in [int, float]:
      raise TypeError('quantity should be of [int, float]')
    return library.humanThrowAmmo(self.ptr, x, y, quantity)
  ##Build or reenforce a wall in an adjacent square
  def build(self, x, y):
    self.validify()
    if x.__class__ not in [int, float]:
      raise TypeError('x should be of [int, float]')
    if y.__class__ not in [int, float]:
      raise TypeError('y should be of [int, float]')
    return library.humanBuild(self.ptr, x, y)
  ##Orders a crate with a weapon produced at x,y
  @staticmethod
  def callWeapon(weapon, x, y):
    if weapon.__class__ not in [Weapon]:
      raise TypeError('weapon should be of [Weapon]')
    weapon.validify()
    if x.__class__ not in [int, float]:
      raise TypeError('x should be of [int, float]')
    if y.__class__ not in [int, float]:
      raise TypeError('y should be of [int, float]')
    return library.humanCallWeapon(weapon.ptr, x, y)
  ##Orders a crate of ammo at x,y
  @staticmethod
  def callAmmo(x, y):
    if x.__class__ not in [int, float]:
      raise TypeError('x should be of [int, float]')
    if y.__class__ not in [int, float]:
      raise TypeError('y should be of [int, float]')
    return library.humanCallAmmo(x, y)
  ##Creates a human at x,y wielding the chosen weapon
  @staticmethod
  def spawn(weapon, x, y):
    if weapon.__class__ not in [Weapon]:
      raise TypeError('weapon should be of [Weapon]')
    weapon.validify()
    if x.__class__ not in [int, float]:
      raise TypeError('x should be of [int, float]')
    if y.__class__ not in [int, float]:
      raise TypeError('y should be of [int, float]')
    return library.humanSpawn(weapon.ptr, x, y)

  ##Unique ID
  def getID(self):
    self.validify()
    return library.humanGetID(self.ptr)
  ##X coordinate
  def getX(self):
    self.validify()
    return library.humanGetX(self.ptr)
  ##Y coordinate
  def getY(self):
    self.validify()
    return library.humanGetY(self.ptr)
  ##Current HP
  def getHp(self):
    self.validify()
    return library.humanGetHp(self.ptr)
  ##Maximum HP
  def getMaxHp(self):
    self.validify()
    return library.humanGetMaxHp(self.ptr)
  ##Moves left this turn
  def getMoves(self):
    self.validify()
    return library.humanGetMoves(self.ptr)
  ##Maximum moves per turn
  def getMaxMoves(self):
    self.validify()
    return library.humanGetMaxMoves(self.ptr)
  ##Weapon ID
  def getWeaponID(self):
    self.validify()
    return library.humanGetWeaponID(self.ptr)
  ##Number of hexes the unit can see
  def getSightRadius(self):
    self.validify()
    return library.humanGetSightRadius(self.ptr)
  ##ID of the owning player.
  def getOwnerID(self):
    self.validify()
    return library.humanGetOwnerID(self.ptr)
  ##Amount of time saved up for using weapons.
  def getIdleTime(self):
    self.validify()
    return library.humanGetIdleTime(self.ptr)
  ##Ammunition on hand
  def getAmmo(self):
    self.validify()
    return library.humanGetAmmo(self.ptr)
  ##Weapon specialty
  def getSpecialty(self):
    self.validify()
    return library.humanGetSpecialty(self.ptr)

##Spawnzone
class Spawnzone(GameObject):
  ##Unique ID
  def getID(self):
    self.validify()
    return library.spawnzoneGetID(self.ptr)
  ##line start x coordinate
  def getX1(self):
    self.validify()
    return library.spawnzoneGetX1(self.ptr)
  ##line start y coordinate
  def getY1(self):
    self.validify()
    return library.spawnzoneGetY1(self.ptr)
  ##line end x coordinate
  def getX2(self):
    self.validify()
    return library.spawnzoneGetX2(self.ptr)
  ##line end y coordinate
  def getY2(self):
    self.validify()
    return library.spawnzoneGetY2(self.ptr)

##Wall
class Wall(GameObject):
  ##Unique ID number
  def getID(self):
    self.validify()
    return library.wallGetID(self.ptr)
  ##X coordinate
  def getX(self):
    self.validify()
    return library.wallGetX(self.ptr)
  ##Y coordinate
  def getY(self):
    self.validify()
    return library.wallGetY(self.ptr)
  ##Current HP
  def getHp(self):
    self.validify()
    return library.wallGetHp(self.ptr)
  ##Maximum HP
  def getMaxHp(self):
    self.validify()
    return library.wallGetMaxHp(self.ptr)

##Weapon
class Weapon(GameObject):
  ##Unique ID
  def getID(self):
    self.validify()
    return library.weaponGetID(self.ptr)
  ##The weapon's name
  def getName(self):
    self.validify()
    return library.weaponGetName(self.ptr)
  ##The number of hexes a target must be within to be attacked
  def getRange(self):
    self.validify()
    return library.weaponGetRange(self.ptr)
  ##The amount of ammo each attack requires with the weapon
  def getAmmo(self):
    self.validify()
    return library.weaponGetAmmo(self.ptr)
  ##The number of unitts of idleTime firing uses
  def getDelay(self):
    self.validify()
    return library.weaponGetDelay(self.ptr)
  ##The damage dealt on an attack
  def getDamage(self):
    self.validify()
    return library.weaponGetDamage(self.ptr)
  ##Damage dealt to all units other than the target in the hex
  def getVerticalSplashDamage(self):
    self.validify()
    return library.weaponGetVerticalSplashDamage(self.ptr)
  ##Damage dealt to all units in the horizontal splash radius
  def getHorizontalSplashDamage(self):
    self.validify()
    return library.weaponGetHorizontalSplashDamage(self.ptr)
  ##Number of hexes from the center horizontal splash spreads
  def getHorizontalSplashRadius(self):
    self.validify()
    return library.weaponGetHorizontalSplashRadius(self.ptr)
  ##True if humans can start with this weapon  
  def getCanSpawnWith(self):
      self.validify()
      return library.weaponGetCanSpawnWith(self.ptr)
  ##True if humans can call for an airdrop with this weapon  
  def getCanCall(self):
    self.validify()
    return library.weaponGetCanCall(self.ptr)

##Zombie
class Zombie(GameObject):
  ##Move forward
  def move(self):
    self.validify()
    return library.zombieMove(self.ptr)
  ##Attack the human
  def attack(self, target):
    self.validify()
    if target.__class__ not in [Human]:
      raise TypeError('target should be of [Human]')
    target.validify()
    return library.zombieAttack(self.ptr, target.ptr)
  ##Attack the wall
  def attackWall(self, target):
    self.validify()
    if target.__class__ not in [Wall]:
      raise TypeError('target should be of [Wall]')
    target.validify()
    return library.zombieAttackWall(self.ptr, target.ptr)
  ##Eat a buddy, healing self 200% of the zombie's hp
  def eat(self, target):
    self.validify()
    if target.__class__ not in [Zombie]:
      raise TypeError('target should be of [Zombie]')
    target.validify()
    return library.zombieEat(self.ptr, target.ptr)
  ##Turn left (1) or right (-1)
  def turn(self, direction):
    self.validify()
    if direction.__class__ not in [int, float]:
      raise TypeError('direction should be of [int, float]')
    return library.zombieTurn(self.ptr, direction)
  ##Create a zombie in the spawn point
  @staticmethod
  def spawn(zone, facing):
    if zone.__class__ not in [Spawnzone]:
      raise TypeError('zone should be of [Spawnzone]')
    zone.validify()
    if facing.__class__ not in [int, float]:
      raise TypeError('facing should be of [int, float]')
    return library.zombieSpawn(zone.ptr, facing)
  ##Unique ID
  def getID(self):
    self.validify()
    return library.zombieGetID(self.ptr)
  ##X coordinate
  def getX(self):
    self.validify()
    return library.zombieGetX(self.ptr)
  ##Y coordinate
  def getY(self):
    self.validify()
    return library.zombieGetY(self.ptr)
  ##Current HP
  def getHp(self):
    self.validify()
    return library.zombieGetHp(self.ptr)
  ##Maximum HP
  def getMaxHp(self):
    self.validify()
    return library.zombieGetMaxHp(self.ptr)
  ##Moves left this turn
  def getMoves(self):
    self.validify()
    return library.zombieGetMoves(self.ptr)
  ##Maximum moves per turn
  def getMaxMoves(self):
    self.validify()
    return library.zombieGetMaxMoves(self.ptr)
  ##Weapon ID
  def getWeaponID(self):
    self.validify()
    return library.zombieGetWeaponID(self.ptr)
  ##Number of hexes the unit can see
  def getSightRadius(self):
    self.validify()
    return library.zombieGetSightRadius(self.ptr)
  ##ID of the owning player.
  def getOwnerID(self):
    self.validify()
    return library.zombieGetOwnerID(self.ptr)
  ##Amount of time saved up for using weapons.
  def getIdleTime(self):
    self.validify()
    return library.zombieGetIdleTime(self.ptr)
  ##Direction the Zombie is can attack or move in
  def getFacing(self):
    self.validify()
    return library.zombieGetFacing(self.ptr)
  ##Scent, see gamemanual.txt
  def getSmell(self):
    self.validify()
    return library.zombieGetSmell(self.ptr)
