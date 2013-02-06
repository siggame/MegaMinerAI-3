from library import library

## \brief A basic AI interface.

##This class implements most the code an AI would need to interface with the lower-level game code.
##AIs should extend this class to get a lot of builer-plate code out of the way
##The provided AI class does just that.
class BaseAI:
  walls = [] ##The visible Walls
  crates = [] ##The visible Crates
  weapons = [] ##All Weapons
  humans = [] ##The Visible Humans
  zombies = [] ## The visible Zombies
  airstrikes = [] ##The Airstrikes (only visible to humans)
  spawnzones = [] ##All spawnzones
  
  iteration = 0

  ##Current turn number.  
  @staticmethod
  def turnNum():
    return library.getTurnNumber()
  ##Size of the map.  
  @staticmethod
  def mapSize():
    return library.getMapSize()
  ##Name of the human player.  
  @staticmethod
  def humanTeamName():
    return library.getHumanTeamName()
  ##Name of the zombie player.  
  @staticmethod
  def zombieTeamName():
    return library.getZombieTeamName()
  ##Time in seconds human player has remaining.
  @staticmethod
  def humanTime():
    return library.getHumanTime()
  ##Time in seconds zombie player has remaining.
  @staticmethod
  def zombieTime():
    return library.getZombieTime()
  ##Number of airstrikes ready to be launched  
  @staticmethod
  def airstrikesReady():
    return library.getAirstrikesReady()
  ##Zombies that can spawn (may exceed cap)  
  @staticmethod
  def zombiesReady():
    return library.getZombiesReady()
  ##Humans that can spawn  
  @staticmethod
  def humansReady():
    return library.getHumansReady()
  ##Number of zombies that may live at a time  
  @staticmethod
  def zombieCap():
    return library.getZombieCap()
  
  def startTurn(self):
    from gameObjects import Wall, Crate, Weapon, Human, Zombie, Airstrike, Spawnzone
    
    BaseAI.walls = [Wall(library.getWalls(i)) for i in xrange(library.getWallCount())]
    BaseAI.crates = [Crate(library.getCrates(i)) for i in xrange(library.getCrateCount())]
    BaseAI.weapons = [Weapon(library.getWeapons(i)) for i in xrange(library.getWeaponCount())]
    BaseAI.humans = [Human(library.getHumans(i)) for i in xrange(library.getHumanCount())]
    BaseAI.zombies = [Zombie(library.getZombies(i)) for i in xrange(library.getZombieCount())]
    BaseAI.airstrikes = [Airstrike(library.getAirstrikes(i)) for i in xrange(library.getAirstrikeCount())]
    BaseAI.spawnzones = [Spawnzone(library.getSpawnzones(i)) for i in xrange(library.getSpawnzoneCount())]
    
    if self.turnNum() == 1:
      self.init()
      if not library.isHuman():
        return 1
    if library.isHuman():
      return self.runHuman()
    else:
      return self.runZombie()