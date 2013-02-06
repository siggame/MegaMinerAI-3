
///BaseAI
public abstract class BaseAI
{
    static Wall[] walls; ///< All walls that are visible during the current turn
    static Crate[] crates; ///< All crates that are visible during the current turn
    static Weapon[] weapons; ///< All weapons in the game
    static Human[] humans; ///< All humans that are visible during the current turn
    static Zombie[] zombies; ///< All zombies that are visible during the current turn
    static Airstrike[] airstrikes; ///< All airstrikes that exist during the current turn
    static Spawnzone[] spawnzones; ///< All spawnzones in the game

    static int iteration;

    static final int DOWN=5;
    static final int DOWN_LEFT=4;
    static final int UP_LEFT=3;
    static final int UP=2;
    static final int UP_RIGHT=1;
    static final int DOWN_RIGHT=0;

    static final int TURN_LEFT=1;
    static final int TURN_RIGHT=-1;

    ///
    ///Make this your username, which should be provided.
    public abstract String username();
    ///
    ///Make this your password, which should be provided.
    public abstract String password();
    ///
    ///This is run on turn 1 before runHuman or runZombie
    public abstract void init();
    ///
    ///This is run every turn when the AI is playing a human. Return true to end the turn, return false
    ///to request a status update from the server and then immediately rerun this function with the
    ///latest game status.
    public abstract boolean runHuman();
    ///
    ///This is run every turn when the AI is playing a zombie. Return true to end the turn, return false
    ///to request a status update from the server and then immediately rerun this function with the
    ///latest game status.
    public abstract boolean runZombie();

    public boolean startTurn()
    {
        int count = 0;
        count = Client.INSTANCE.getWallCount();
        walls = new Wall[count];
        for(int i = 0; i < count; i++)
        {
            walls[i] = new Wall(Client.INSTANCE.getWalls(i));
        }
        count = Client.INSTANCE.getCrateCount();
        crates = new Crate[count];
        for(int i = 0; i < count; i++)
        {
            crates[i] = new Crate(Client.INSTANCE.getCrates(i));
        }
        count = Client.INSTANCE.getWeaponCount();
        weapons = new Weapon[count];
        for(int i = 0; i < count; i++)
        {
            weapons[i] = new Weapon(Client.INSTANCE.getWeapons(i));
        }
        count = Client.INSTANCE.getHumanCount();
        humans = new Human[count];
        for(int i = 0; i < count; i++)
        {
            humans[i] = new Human(Client.INSTANCE.getHumans(i));
        }
        count = Client.INSTANCE.getZombieCount();
        zombies = new Zombie[count];
        for(int i = 0; i < count; i++)
        {
            zombies[i] = new Zombie(Client.INSTANCE.getZombies(i));
        }
        count = Client.INSTANCE.getAirstrikeCount();
        airstrikes = new Airstrike[count];
        for(int i = 0; i < count; i++)
        {
            airstrikes[i] = new Airstrike(Client.INSTANCE.getAirstrikes(i));
        }
        count = Client.INSTANCE.getSpawnzoneCount();
        spawnzones = new Spawnzone[count];
        for(int i = 0; i < count; i++)
        {
            spawnzones[i] = new Spawnzone(Client.INSTANCE.getSpawnzones(i));
        }

        iteration++;

        if(turnNum() == 1)
        {
          init();
          if(!Client.INSTANCE.isHuman())
		  {
		    return true;
          }
        }

        if(Client.INSTANCE.isHuman())
        {
            return runHuman();
        }
        else
        {
            return runZombie();
        }
    }
   
    ///Current turn number.
    public int turnNum()
    {
        return Client.INSTANCE.getTurnNumber();
    }
    
    ///Size of the map.
    public int mapSize()
    {
      return Client.INSTANCE.getMapSize();
    }
  
    ///Name of the human player.
    public String humanTeamName()
    {
      return Client.INSTANCE.getHumanTeamName();
    }
    
    ///Name of the zombie player.
    public String  zombieTeamName()
    {
      return Client.INSTANCE.getZombieTeamName();
    }

    ///Time in seconds human player has remaining.
    public float humanTime()
    {
      return Client.INSTANCE.getHumanTime();
    }

    ///Time in seconds zombie player has remaining.
    public  float zombieTime()
    {
      return Client.INSTANCE.getZombieTime();
    }

    ///Number of airstrikes ready to be launched
    public float airstrikesReady()
    {
      return Client.INSTANCE.getAirstrikesReady();
    }

    ///Zombies that can spawn (may exceed cap)
    public int zombiesReady()
    {
      return Client.INSTANCE.getZombiesReady();
    }

    ///Humans that can spawn
    public int humansReady()
    {
      return Client.INSTANCE.getHumansReady();
    }

    ///Number of zombies that may live at a time
    public int zombieCap()
    {
      return Client.INSTANCE.getZombieCap();
    }
}
