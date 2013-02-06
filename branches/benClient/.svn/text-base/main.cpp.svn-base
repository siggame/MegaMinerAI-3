#include <iostream>

#include <cstring>

#include "AI.h"
#include "network.h"
#include "game.h"

#include "sexp/sexp.h"

using namespace std;

int main(int argc, char** argv)
{
  const char* message;
  sexp_t* expression;
  AI ai;
  int socket = open_server_connection(argv[1], "19000");
  if(socket == -1)
  {
    cerr << "Unable to connect to server" << endl;
    return 1;
  }
  login(socket, ai.username(), ai.password());
  expression = extract_sexpr(rec_string(socket));
  expression = expression->list;
  if(expression->val == NULL || strcmp(expression->val, "login-accepted") != 0)
  {
    cerr << "Unable to login to server" << endl;
    return 1;
  }
  if(argc < 3)
  {
    createGame();
  }
  else
  {
    joinGame(atoi(argv[2]));
  }
  while(networkLoop(socket))
  {
    if(ai.startTurn())
    {
      endTurn();
    }
    else
    {
      getStatus();
    }
  }
  return 0;
}
