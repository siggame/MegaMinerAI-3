import com.sun.jna.Pointer;

class Spawnzone
{
    Pointer ptr;
    int ID;
    int iteration;
    public Spawnzone(Pointer p)
    {
      ptr = p;
      ID = Client.INSTANCE.spawnzoneGetID(ptr);
      iteration = BaseAI.iteration;
    }
    
    boolean validify()
    {
      if(iteration == BaseAI.iteration) return true;
      for(int i = 0; i < BaseAI.spawnzones.length; i++)
      {
        if(BaseAI.spawnzones[i].ID == ID)
        {
          ptr = BaseAI.spawnzones[i].ptr;
          iteration = BaseAI.iteration;
          return true;
        }
      }
      throw new ExistentialError();
    }
    ///Unique ID    
    public int getID()
    {
      return Client.INSTANCE.spawnzoneGetID(ptr);
    }
    ///line start x coordinate
    public int getX1()
    {
      return Client.INSTANCE.spawnzoneGetX1(ptr);
    }
    ///line start y coordinate
    public int getY1()
    {
      return Client.INSTANCE.spawnzoneGetY1(ptr);
    }
    ///line end x coordinate
    public int getX2()
    {
      return Client.INSTANCE.spawnzoneGetX2(ptr);
    }
    ///line end y coordinate
    public int getY2()
    {
      return Client.INSTANCE.spawnzoneGetY2(ptr);
    }
}
 
