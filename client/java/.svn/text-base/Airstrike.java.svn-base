import com.sun.jna.Pointer;

///Airstrike
class Airstrike
{
    Pointer ptr;
    int ID;
    int iteration;
    public Airstrike(Pointer p)
    {
      ptr = p;
      ID = Client.INSTANCE.airstrikeGetID(ptr);
      iteration = BaseAI.iteration;
    }

    boolean validify()
    {
      if(iteration == BaseAI.iteration) return true;
      for(int i = 0; i < BaseAI.airstrikes.length; i++)
      {
        if(BaseAI.airstrikes[i].ID == ID)
        {
          ptr = BaseAI.airstrikes[i].ptr;
          iteration = BaseAI.iteration;
          return true;
        }
      }
      throw new ExistentialError();
    }

    ///Unique ID
    public int getID()
    {
      validify();
      return Client.INSTANCE.airstrikeGetID(ptr);
    }

    ///X coordinate
    public int getX()
    {
      validify();
      return Client.INSTANCE.airstrikeGetX(ptr);
    }

    ///Y coordinate
    public int getY()
    {
      validify();
      return Client.INSTANCE.airstrikeGetY(ptr);
    }

    ///Turns until landing
    public int getDelay()
    {
      validify();
      return Client.INSTANCE.airstrikeGetDelay(ptr);
    }

    ///Weapon being dropped (ammo == -1)
    public int getItemID()
    {
      validify();
      return Client.INSTANCE.airstrikeGetItemID(ptr);
    }
}

