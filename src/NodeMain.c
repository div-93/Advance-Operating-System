#include "NodeMain.h"
#include "NodeCommon.h"

int main(int argc, char * argv[])
{
  pthread_t thserver;
  pthread_t thclient;
  pthread_t thcli;
  
  unsigned short in_cli;
  unsigned short in_clnt;
  unsigned short  export;

  export = atoi(argv[1]);
  in_clnt = export+1;
  in_cli = in_clnt+1;

  if(pthread_create(&thcli,NULL,nodeCli,&in_cli) == 0)
    printf("\nNODE MAIN: CLI THREAD CREATED\n");
  else
  {
    perror("\nNODE MAIN: CLI THREAD CREATION FAIL\n");
    return FAILURE;
  }

  if(pthread_create(&thserver,NULL,nodeServ,&export) == 0)
    printf("\nNODE MAIN: SERVER THREAD CREATED\n");
  else
  {
    perror("\nNODE MAIN: SERVER THREAD CREATION FAIL\n");
    return FAILURE;
  }
 
  if(pthread_create(&thclient,NULL,nodeClnt,&in_clnt) == 0)
    printf("\nNODE MAIN: CLIENT THREAD CREATED\n");
  else
  {
    perror("\nNODE MAIN: CLNT THREAD CREATION FAIL\n");
     return FAILURE;
  }
  
  pthread_join(thcli,NULL);
  pthread_join(thclient,NULL);
  pthread_join(thserver,NULL);

  return SUCCESS;
}
