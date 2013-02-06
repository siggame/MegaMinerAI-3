#include "AI.h"

bool AI::runHuman()
{
  if(turnNum() == 1)
  {
    for(int i=0;i<getHumansReady();i++)
    {
      Human::spawn(weapons[1], 0, 2);
    }
  }
  else
  {
    if(humans[0].x() == 0)
	{
      humans[0].move(humans[0].x()+1, humans[0].y());
	}
	else
	{
      humans[0].move(humans[0].x()-1, humans[0].y());
	}
  }
  return true;
} 
