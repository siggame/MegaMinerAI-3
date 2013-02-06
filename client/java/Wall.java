import com.sun.jna.Pointer;

///Wall
class Wall
{
    Pointer ptr;
    int ID;
    int iteration;
    public Wall(Pointer p)
    {
      ptr = p;
      ID = Client.INSTANCE.wallGetID(ptr);
      iteration = BaseAI.iteration;
    }
    
    boolean validify()
    {
      if(iteration == BaseAI.iteration) return true;
      for(int i = 0; i < BaseAI.walls.length; i++)
      {
        if(BaseAI.walls[i].ID == ID)
        {
          ptr = BaseAI.walls[i].ptr;
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
      return Client.INSTANCE.wallGetID(ptr);
    }
    ///X coordinate
    public int getX()
    {
      validify();
      return Client.INSTANCE.wallGetX(ptr);
    }
    ///Y coordinate
    public int getY()
    {
      validify();
      return Client.INSTANCE.wallGetY(ptr);
    }
    ///Current HP
    public int getHp()
    {
      validify();
      return Client.INSTANCE.wallGetHp(ptr);
    }
    ///Maximum HP
    public int getMaxHp()
    {
      validify();
      return Client.INSTANCE.wallGetMaxHp(ptr);
    }
}
 
