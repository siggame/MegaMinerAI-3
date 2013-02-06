import com.sun.jna.Pointer;

///Zombie
class Zombie
{
    Pointer ptr;
    int ID;
    int iteration;
    public Zombie(Pointer p)
    {
            ptr = p;
            ID = Client.INSTANCE.zombieGetID(ptr);
            iteration = BaseAI.iteration;
    }

    boolean validify()
    {
        if(iteration == BaseAI.iteration) return true;
        for(int i = 0; i < BaseAI.zombies.length; i++)
        {
            if(BaseAI.zombies[i].ID == ID)
            {
                ptr = BaseAI.zombies[i].ptr;
                iteration = BaseAI.iteration;
                return true;
            }
        }
      throw new ExistentialError();
    }
    ///Moves left this turn
    public boolean move()
    {
        validify();
        return Client.INSTANCE.zombieMove(ptr);
    }
    ///Attack the human
    public boolean attack(Human target)
    {
        validify();
        target.validify();
        return Client.INSTANCE.zombieAttack(ptr, target.ptr);
    }
    ///Attack the wall
    public boolean attackWall(Wall target)
    {
        validify();
        target.validify();
        return Client.INSTANCE.zombieAttackWall(ptr, target.ptr);
    }
    ///Eat a buddy, healing self 200% of the zombie's hp
    public boolean eat(Zombie target)
    {
        validify();
        target.validify();
        return Client.INSTANCE.zombieEat(ptr, target.ptr);
    }
    ///Turn left (1) or right (-1)
    public boolean turn(int direction)
    {
        validify();
        return Client.INSTANCE.zombieTurn(ptr, direction);
    }
    ///Create a zombie is the spawn point
    public static boolean spawn(Spawnzone zone, int facing)
    {
        zone.validify();
        return Client.INSTANCE.zombieSpawn(zone.ptr, facing);
    }
    ///Unique ID
    public int getID()
    {
        validify();
        return Client.INSTANCE.zombieGetID(ptr);
    }
    ///X coordinate
    public int getX()
    {
        validify();
        return Client.INSTANCE.zombieGetX(ptr);
    }
    ///Y coordinate
    public int getY()
    {
        validify();
        return Client.INSTANCE.zombieGetY(ptr);
    }
    ///Current HP
    public int getHp()
    {
        validify();
        return Client.INSTANCE.zombieGetHp(ptr);
    }
    ///Maximum HP
    public int getMaxHp()
    {
        validify();
        return Client.INSTANCE.zombieGetMaxHp(ptr);
    }
    ///Moves left this turn
    public int getMoves()
    {
        validify();
        return Client.INSTANCE.zombieGetMoves(ptr);
    }
    ///Maximum moves per turn
    public int getMaxMoves()
    {
        validify();
        return Client.INSTANCE.zombieGetMaxMoves(ptr);
    }
    ///Weapon ID
    public int getWeaponID()
    {
        validify();
        return Client.INSTANCE.zombieGetWeaponID(ptr);
    }
    ///Number of hexes the unit can see
    public int getSightRadius()
    {
        validify();
        return Client.INSTANCE.zombieGetSightRadius(ptr);
    }
    ///ID of the owning player.
    public int getOwnerID()
    {
        validify();
        return Client.INSTANCE.zombieGetOwnerID(ptr);
    }
    ///Amount of time saved up for using weapons.
    public float getIdleTime()
    {
        validify();
        return Client.INSTANCE.zombieGetIdleTime(ptr);
    }
    ///Direction the Zombie is can attack or move in
    public int getFacing()
    {
        validify();
        return Client.INSTANCE.zombieGetFacing(ptr);
    }
    ///Scent, see gamemanual.txt
    public float getSmell()
    {
        validify();
        return Client.INSTANCE.zombieGetSmell(ptr);
    }
}
