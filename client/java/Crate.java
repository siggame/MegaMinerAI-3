import com.sun.jna.Pointer;

///Crate
class Crate
{
    Pointer ptr;
    int ID;
    int iteration;
    public Crate(Pointer p)
    {
      ptr = p;
      ID = Client.INSTANCE.crateGetID(ptr);
      iteration = BaseAI.iteration;
    }

    boolean validify()
    {
      if(iteration == BaseAI.iteration) return true;
      for(int i = 0; i < BaseAI.crates.length; i++)
      {
        if(BaseAI.crates[i].ID == ID)
        {
          ptr = BaseAI.crates[i].ptr;
          iteration = BaseAI.iteration;
          return true;
        }
      }
      throw new ExistentialError();
    }

    ///Unique ID number
    public int getID()
    {
      validify();
      return Client.INSTANCE.crateGetID(ptr);
    }

    ///X coordinate
    public int getX()
    {
      validify();
      return Client.INSTANCE.crateGetX(ptr);
    }

    ///Y coordinate
    public int getY()
    {
      validify();
      return Client.INSTANCE.crateGetY(ptr);
    }

    ///Current HP
    public int getHp()
    {
      validify();
      return Client.INSTANCE.crateGetHp(ptr);
    }

    ///Maximum HP
    public int getMaxHp()
    {
      validify();
      return Client.INSTANCE.crateGetMaxHp(ptr);
    }

    ///Contents of the crate; Weapon ID or -1 for ammo
    public int getContent()
    {
      validify();
      return Client.INSTANCE.crateGetContent(ptr);
    }
}

