import com.sun.jna.Pointer;

///Human
class Human
{
    Pointer ptr;
    int ID;
    int iteration;
    public Human(Pointer p)
    {
      ptr = p;
      ID = Client.INSTANCE.humanGetID(ptr);
      iteration = BaseAI.iteration;
    }

    boolean validify()
    {
      if(iteration == BaseAI.iteration) return true;
      for(int i = 0; i < BaseAI.humans.length; i++)
      {
        if(BaseAI.humans[i].ID == ID)
        {
          ptr = BaseAI.humans[i].ptr;
          iteration = BaseAI.iteration;
          return true;
        }
      }
      throw new ExistentialError();
    }

    ///Move to the adjacent hex at x y
    public boolean move(int x, int y)
    {
        validify();
        return Client.INSTANCE.humanMove(ptr, x, y);
    }

    ///Attack the Zombie
    public boolean attack(Zombie target)
    {
        validify();
        target.validify();
        return Client.INSTANCE.humanAttack(ptr, target.ptr);
    }

    ///Attack the Crate
    public boolean attackCrate(Crate target)
    {
        validify();
        target.validify();
        return Client.INSTANCE.humanAttackCrate(ptr, target.ptr);
    }

    ///Attack the Wall
    public boolean attackWall(Wall target)
    {
        validify();
        target.validify();
        return Client.INSTANCE.humanAttackWall(ptr, target.ptr);
    }

    ///Attack the ground
    public boolean attackGround(int x, int y)
    {
        validify();
        return Client.INSTANCE.humanAttackGround(ptr, x, y);
    }

    ///Take the contents of a crate in the same hex
    public boolean grab(Crate target)
    {
        validify();
        target.validify();
        return Client.INSTANCE.humanGrab(ptr, target.ptr);
    }
    ///Take the contents of a crate in the same hex
    public boolean grab(Human target, int quantity)
    {
        validify();
        target.validify();
        return Client.INSTANCE.humanGive(ptr, target.ptr, quantity);
    }

    ///Drops equipped weapon in an adjacent hex
    public boolean throwWeapon(int x, int y)
    {
        validify();
        return Client.INSTANCE.humanThrowWeapon(ptr, x, y);
    }

    ///Drops an amount of ammo in an adjacent hex
    public boolean throwAmmo(int x, int y, int quantity)
    {
        validify();
        return Client.INSTANCE.humanThrowAmmo(ptr, x, y, quantity);
    }

    ///Build or reenforce a wall in an adjacent square
    public boolean build(int x, int y)
    {
        validify();
        return Client.INSTANCE.humanBuild(ptr, x, y);
    }

    ///Orders a crate with a weapon produced at x,y
    public static boolean callWeapon(Weapon weapon, int x, int y)
    {
        weapon.validify();
        return Client.INSTANCE.humanCallWeapon(weapon.ptr, x, y);
    }

    ///Orders a crate of ammo at x,y
    public static boolean callAmmo(int x, int y)
    {
        return Client.INSTANCE.humanCallAmmo(x, y);
    }

    ///Creates a human at x,y wielding the chosen weapon
    public static boolean spawn(Weapon weapon, int x, int y)
    {
        weapon.validify();
        return Client.INSTANCE.humanSpawn(weapon.ptr, x, y);
    }

    ///Unique ID
    public int getID()
    {
        validify();
        return Client.INSTANCE.humanGetID(ptr);
    }

    ///X coordinate
    public int getX()
    {
        validify();
        return Client.INSTANCE.humanGetX(ptr);
    }

    ///Y coordinate
    public int getY()
    {
        validify();
        return Client.INSTANCE.humanGetY(ptr);
    }
    ///Current HP
    public int getHp()
    {
        validify();
        return Client.INSTANCE.humanGetHp(ptr);
    }
    ///Maximum HP
    public int getMaxHp()
    {
        validify();
        return Client.INSTANCE.humanGetMaxHp(ptr);
    }
    ///Moves left this turn
    public int getMoves()
    {
        validify();
        return Client.INSTANCE.humanGetMoves(ptr);
    }
    ///Maximum moves per turn
    public int getMaxMoves()
    {
        validify();
        return Client.INSTANCE.humanGetMaxMoves(ptr);
    }
    ///Weapon ID
    public int getWeaponID()
    {
        validify();
        return Client.INSTANCE.humanGetWeaponID(ptr);
    }
    ///Number of hexes the unit can see
    public int getSightRadius()
    {
        validify();
        return Client.INSTANCE.humanGetSightRadius(ptr);
    }
    ///ID of the owning player.
    public int getOwnerID()
    {
        validify();
        return Client.INSTANCE.humanGetOwnerID(ptr);
    }
    ///Amount of time saved up for using weapons.
    public float getIdleTime()
    {
        validify();
        return Client.INSTANCE.humanGetIdleTime(ptr);
    }
    ///Ammunition on hand
    public int getAmmo()
    {
        validify();
        return Client.INSTANCE.humanGetAmmo(ptr);
    }
    ///Weapon specialty
    public int getSpecialty()
    {
        validify();
        return Client.INSTANCE.humanGetSpecialty(ptr);
    }
}
