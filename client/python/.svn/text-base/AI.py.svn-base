from BaseAI import BaseAI
from gameObjects import *
import random

DOWN=5
DOWN_LEFT=4
UP_LEFT=3
UP=2
UP_RIGHT=1
DOWN_RIGHT=0


TURN_LEFT=1
TURN_RIGHT=-1

##The class implementing game logic.
class AI(BaseAI):
  ##
  ##Make this your username, which should be provided.
  @staticmethod
  def username():
    return "Shell AI"
  ##
  ##Make this your password, which should be provided.  
  @staticmethod
  def password():
    return "password"
  ##
  ##This is run every turn when the AI is playing a human. Return true to end the turn, return false
  ##to request a status update from the server and then immediately rerun this function with the
  ##latest game status.
  def runHuman(self):
    print self.turnNum()
    if self.turnNum() == 1:
      Human.spawn(self.weapons[0],0,2)
    else:
      if self.humans[0].getX() == 0:
        self.humans[0].move(self.humans[0].getX()+1, self.humans[0].getY())
      else:
        self.humans[0].move(self.humans[0].getX()-1, self.humans[0].getY())
    return 1
  ##
  ##This is run every turn when the AI is playing a zombie. Return true to end the turn, return false
  ##to request a status update from the server and then immediately rerun this function with the
  ##latest game status.
  def runZombie(self):
    print self.turnNum()
    if self.zombiesReady():
      Zombie.spawn(self.spawnzones[0],0)
    for i in self.zombies:
      move = random.randrange(3)
      if move == 0:
        i.move();
      if move == 1:
        i.turn(-1)
      if move == 2:
        i.turn(1)
    return 1
  ##
  ##This is run on turn 1 before runHuman or runZombie
  def init(self):
    pass
