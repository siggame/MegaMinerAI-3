#ifndef NETWORK_H
#define NETWORK_H

#include "sexp/sexp.h"
extern "C"
{
  int open_server_connection(const char* host, const char* port);
  int send_string(int socket, const char* payload);
  const char* rec_string(int socket);
  
  sexp_t* extract_sexpr(const char* sexpr);
}
#endif
