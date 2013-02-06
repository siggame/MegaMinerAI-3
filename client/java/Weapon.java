import com.sun.jna.Pointer;

///Weapon
class Weapon
{
    Pointer ptr;
    int ID;
    int iteration;
    public Weapon(Pointer p)
    {
      ptr = p;
      ID = Client.INSTANCE.weaponGetID(ptr);
      iteration = BaseAI.iteration;
    }

    boolean validify()
    {
      if(iteration == BaseAI.iteration) return true;
      for(int i = 0; i < BaseAI.weapons.length; i++)
      {
        if(BaseAI.weapons[i].ID == ID)
        {
          ptr = BaseAI.weapons[i].ptr;
          iteration = BaseAI.iteration;
          return true;
        }
      }
      throw new ExistentialError();
    }

    public int getID()
    {
      validify();
      return Client.INSTANCE.weaponGetID(ptr);
    }

    public String getName()
    {
      validify();
      return Client.INSTANCE.weaponGetName(ptr);
    };

    public int getRange()
    {
      validify();
      return Client.INSTANCE.weaponGetRange(ptr);
    };

    public int getAmmo()
    {
      validify();
      return Client.INSTANCE.weaponGetAmmo(ptr);
    };

    public float getDelay()
    {
      validify();
      return Client.INSTANCE.weaponGetDelay(ptr);
    };

    public int getDamage()
    {
      validify();
      return Client.INSTANCE.weaponGetDamage(ptr);
    };

    public int getVerticalSplashDamage()
    {
      validify();
      return Client.INSTANCE.weaponGetVerticalSplashDamage(ptr);
    };

    public int getHorizontalSplashDamage()
    {
      validify();
      return Client.INSTANCE.weaponGetHorizontalSplashDamage(ptr);
    };

    public int getHorizontalSplashRadius()
    {
      validify();
      return Client.INSTANCE.weaponGetHorizontalSplashRadius(ptr);
    };

    public boolean getCanSpawnWith()
	{
	  validify();
	  return Client.INSTANCE.weaponGetCanSpawnWith(ptr);
	};

	public boolean getCanCall()
	{
	  validify();
	  return Client.INSTANCE.weaponGetCanCall(ptr);
    };
}

