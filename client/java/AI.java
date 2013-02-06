import java.util.Random;

///AI
class AI extends BaseAI
{
  public String username()
  {
    return "Shell AI";
  }
  public String password()
  {
    return "password";
  }
  public boolean runHuman()
  {
    if(turnNum() == 1)
    {
      Human.spawn(weapons[0],0,2);
    }
    else
    {
      if(humans[0].getX() == 0)
      {
        humans[0].move(humans[0].getX()+1, humans[0].getY());
      }
      else
	  {
	    humans[0].move(humans[0].getX()-1, humans[0].getY());
	  }
    }
    return true;
  }

  public boolean runZombie()
  {
    java.util.Random rand = new java.util.Random();
    if(zombiesReady() > 0)
    {
      Zombie.spawn(spawnzones[0],0);
    }
    for(int i=0; i < zombies.length; i++)
    {
      int move;
      move = rand.nextInt(3);
      if(move == 0)
	    zombies[i].move();
	  if(move == 1)
	    zombies[i].turn(-1);
	  if(move == 2)
	    zombies[i].turn(1);
	}
    return true;
  }

  public void init() {}
}
