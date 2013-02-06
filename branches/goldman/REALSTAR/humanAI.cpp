#include "AI.h"

bool AI::runHuman()
{
  
  if(turnNum() == 1)
  {   
    Human::spawn(weapons[0], 1, 0);
    Human::spawn(weapons[0], 2, 0);
    Human::spawn(weapons[0], 3, 0);
    Human::spawn(weapons[0], 4, 0);
    Human::spawn(weapons[0], 5, 0);
    Human::spawn(weapons[0], 6, 0);
  }
  else 
  {
    //cout<<"Above blocking/danger"<<endl;
    vector< vector <mpair> > blocking = getBlocking();
    vector< vector <mpair> > danger = getDanger();
    //cout<<"After blocking/danger"<<endl;
    for(unsigned int i=0;i<humans.size();i++)
    {
      //cout<<"\tTop of human["<<i<<"]"<<endl;
      cout<<"Humans["<<i<<"] idle"<<humans[i].idleTime()<<" moves"<<humans[i].moves()<<endl;
      fHuman fake(humans[i]);
      cout<<"Fake idle"<<fake.idleTime()<<" moves "<<fake.moves()<<endl;
      //cout<<"\tFake Made"<<endl;
      vector<lpair> actions = moveHuman(blocking,danger,fake);
      //cout<<"\tPast moveHuman"<<endl;
      if(actions.size() >0)
      {
        //cout<<"\tInto makeMoves"<<endl;
        makeMoves(actions,humans[i]);
        //cout<<"\tPast makeMoves "<<humans[i].id()<<endl;
        char junk;
        //cin>>junk;
        return false;
      }
    }
  }
  return true;
}
vector<lpair> AI::generateSuccessor(vector< vector <mpair> > & blocking, fHuman realstar)
{
  //cout<<"\t\tTop of generateSucc"<<endl;
  //moves, HEAL, ATKZ, BLD
  vector<lpair> returnVec;
  if(realstar.moves() > 0)
  {
    //for adjacent
    for(int i=0;i<6;i++)
    {
      location delta(realstar.x()+offset[i].x+15,realstar.y()+offset[i].y+15);

      //if you can build a wall there
      if(blocking[delta.x][delta.y].type ==EMPTY || blocking[delta.x][delta.y].type == CRATE ||blocking[delta.x][delta.y].type == WALL )
      {
        
        //cout<<"Pushing a build wall on "<<delta.x-15<<","<<delta.y-15<<endl;
        returnVec.push_back(lpair(BLD,delta));
        //if you can move
        if(blocking[delta.x][delta.y].type !=WALL)
        {
          returnVec.push_back(lpair(MOVE,delta));
        }
      }
    }
  }
  //heals
  for(unsigned int i=0;i<humans.size();i++)
  {
    //if you can use your weapon
    if(weapons[realstar.weaponID()].delay() < realstar.idleTime() && realstar.ammo() > weapons[realstar.weaponID()].ammo())
    {
      cout<<"Weapon Attack Added"<<weapons[realstar.weaponID()].delay()<< " "<< realstar.idleTime() <<" "<< humans[i].ammo() <<" "<< weapons[realstar.weaponID()].ammo()<<endl;
      //if you have a med kit && not closed
      if(humans[i].weaponID() == 0)
      {
//        //cout<<"Med kit"<<endl;
        //if it is in range
        if(distance(humans[i].x(),humans[i].y(),realstar.x(),realstar.y())<= weapons[realstar.weaponID()].range())
        {
  //        //cout<<"distance valid"<<endl;
          //if you should heal them
          if(humans[i].hp() - weapons[0].verticalSplashDamage()<= humans[i].maxHp() )
          {
    //        //cout<<"Adding a heal"<<endl;
            returnVec.push_back(lpair(HEAL,location(humans[i].x(),humans[i].y())));
          }
        }
      }
    }
  }
  //attack zombies
  for(unsigned int i=0;i<zombies.size();i++)
  {
    if(distance(zombies[i].x(),zombies[i].y(),realstar.x(),realstar.y()<= weapons[realstar.weaponID()].range()))
    {
      returnVec.push_back(lpair(ATKZ,i));      
    }
  }
  //cout<<"\t\tBottom of generateSucc"<<endl;
  return returnVec;
}

vector<lpair> AI::moveHuman(vector< vector<mpair> > & blocking, vector<vector<mpair> > & danger, fHuman& realstar)
{
  //cout<<"\tTop of moveHuman"<<endl;
  //open lists
  priority_queue<hmpair> open;
  
  //push the current onto open
  hmpair current,ret;
  current.guy = realstar;
  open.push(current);
  ret=current;
  //set up a best
  int bestFound = -9999;
  while(open.size()>0 && bestFound<10)
  {
    //cout<<"\t\tWhile loop"<<endl;
    open.pop();
    vector<lpair> moves = generateSuccessor(blocking,current.guy);
    for(unsigned int i=0;i<moves.size();i++)
    {
      hmpair next;
      next = makeMove(current,blocking,danger,moves[i]);
      open.push(next);
    }
    current = open.top();
    if(current.score > bestFound)
    {
      bestFound = current.score;
      ret = current;
    }
    //cout<<"\t\t/WhileLoop"<<endl;
  }
  //cout<<"\tBottom of move human"<<endl;
  return ret.pastMoves;
}

hmpair AI::makeMove(hmpair cur,vector<vector<mpair> >& blocking,vector<vector<mpair> >& danger, lpair& move)
{
  hmpair current = cur;
  //cout<<"\t\tTop of fakeMove"<<endl;
  location adj = location(move.loc.x-15,move.loc.y-15);
  //cout<<"\t\tADJ:"<<adj.x<<","<<adj.y<<endl;
  //cout<<"\t\tWPN:"<<current.guy.weaponID()<<endl;  
  //cout<<"\t\tMVE:"<<move.type<<endl;
  switch (move.type)
  {
    case BLD:
      current.wallsBuilt+=7;
      current.guy.build(adj.x,adj.y);
      break;
    case MOVE:
      //cout<<"\t\t\tIn MOVE case"<<endl;
      //cout<<"\t\t\tLoc: "<<move.loc.x<<","<<move.loc.y<<endl;
      if(danger[move.loc.x][move.loc.y].type == ZATTACK || danger[move.loc.x][move.loc.y].type == ZSIGHT)
      {
        //cout<<"In ifstae"<<endl;
        current.danger = -danger[move.loc.x][move.loc.y].value;
      }
      //cout<<"past"<<endl;
      current.guy.move(adj.x,adj.y);
      //cout<<"\t\t\tEND MOVE CASE"<<endl;
      break;
    case HEAL:
      //since its negative
      current.healsDone-=weapons[current.guy.weaponID()].verticalSplashDamage();
      current.guy.attack(adj.x,adj.y);
      break;
    case ATKZ:
      current.damageDone+=weapons[current.guy.weaponID()].damage()
                        +weapons[current.guy.weaponID()].verticalSplashDamage()
                        +weapons[current.guy.weaponID()].horizontalSplashDamage();
      current.guy.attack(adj.x,adj.y);
      break;
    default:
      break;
  }
  //cout<<"\t\tAbove pushback"<<endl;
  current.pastMoves.push_back(move);
  //cout<<"\t\tafterPush back"<<endl;
  current.score=current.wallsBuilt+current.damageDone+current.healsDone+current.danger*10;
  //cout<<"\t\tBottom of fake move"<<endl;
  return current;
  
}
int AI::lowestHP(location loc)
{
  int health = 999999;
  int index=-1;
  for(int i=0;i<int(zombies.size());i++)
  {
    if(zombies[i].x() == loc.x && zombies[i].y() == loc.y)
    {
      if(zombies[i].hp() < health && zombies[i].hp() > 0)
      {
        index = i;
      }
    }
  }
  return index;
}

void AI::makeMoves(vector<lpair> &actions,Human&human)
{
  cout<<"-----Begin move Stack----"<<endl;
  int target;
  for(unsigned int i=0;i<actions.size();i++)
  {

    cout<<"Move["<<i<<"] "<<actions[i].type<<" ";
    location adj = location(actions[i].loc.x-15,actions[i].loc.y-15);
    switch (actions[i].type)
    {
      case BLD:
        cout<<"Build called "<<adj.x<<","<<adj.y<<" byHuman: "<<human.x()<<","<<human.y()<<endl;
        human.build(adj.x,adj.y);
        for(unsigned int h=0;h<humans.size();h++){if(humans[h].x() == adj.x && humans[h].y() == adj.y)cout<<"Human: "<<humans[h].id()<<" in way "<<endl;};
        break;
      case MOVE:
        cout<<"Move called "<<adj.x<<","<<adj.y<<" byHuman: "<<human.x()<<","<<human.y()<<endl;
        human.move(adj.x,adj.y);
        cout<<"After: "<<human.x()<<","<<human.y()<<endl;
        break;
      case HEAL:
        cout<<"Healing"<<endl;
        human.attack(adj.x,adj.y);        
        break;
      case ATKZ:
        cout<<"Attacking"<<endl;
        target = lowestHP(adj);
        if(target == -1)
        {
          cout<<"WTF HAPPENED"<<endl;
        }
        else
        {
          human.attack(zombies[i]);
        }
        break;
      default:
        break;
    };
    
 
  }
  cout<<"------End move Stack-----"<<endl;
}
