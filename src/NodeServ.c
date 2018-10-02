#include "NodeServ.h"

void initSerSockfds(int * client_sockets)
{
  int i;
 
  for(i=0;i<MAX_NEIGHBORS;i++)
    client_sockets[i] = 0;

  return;
}

void * nodeServ(void * args)
{
  struct node_mssg mssg;
  int i,j;
  int client_sockets[MAX_NEIGHBORS]; 
  int in_clnt_fd,in_cli_fd,exfd,acceptfd,sd,max_sd,activity;
  struct sockaddr_in cliaddr;
  socklen_t clilen=0;
  fd_set readfds;
  unsigned short *temp;
  unsigned short in_clnt;
  unsigned short in_cli;
  unsigned short export;

  temp = (unsigned short *)args;
  
  export = *temp;
  in_clnt = export+1;
  in_cli = in_clnt+1;
  initSerSockfds(client_sockets);
 
  in_clnt_fd = nodeBind("127.0.0.1",in_clnt);
  if(in_clnt_fd == FAILURE)
  {
    printf("\nNODE SERVER: BIND FAIL FOR 127.0.0.1\n");
    return NULL;
  }
  else
    printf("\nNODE SERVER: BIND SUCCESS FOR 127.0.0.1 PORT:%d\n",in_clnt);
 
  in_cli_fd = nodeBind("127.0.0.1",in_cli);
  if(in_cli_fd == FAILURE)
  {
    printf("\nNODE SERVER: BIND FAIL FOR 127.0.0.1 PORT:%d\n",in_cli);
    return NULL;
  }
  else
  printf("\nNODE SERVER: BIND SUCCESS FOR 127.0.0.1 PORT:%d\n",in_cli);

 
  in_clnt_fd = accept(in_clnt_fd,(struct sockaddr *)&cliaddr,&clilen);
  if(in_clnt_fd < 0)
  {
    printf("\nNODE SERV: CLIENT THRD ACCEPT FAILED\n");
    return FAILURE;
  }
  client_sockets[0] = in_clnt_fd;  
  
  in_cli_fd = accept(in_cli_fd,(struct sockaddr *)&cliaddr,&clilen);
  if(in_cli_fd < 0)
  {
    printf("\nNODE SERV: CLI THRD ACCEPT FAILED\n");
    return FAILURE;
  }
  client_sockets[1] = in_cli_fd;

  exfd = nodeBind("127.0.0.1",export);
  if(exfd == FAILURE)
  {
    printf("\nNODE SERVER: BIND FAIL FOR %s PORT:%d\n","192.168.121.146",export);
    return NULL;
  }
  else
    printf("\nNODE SERVER: BIND SUCCESS FOR %s PORT:%d\n","192.168.121.146",export);


  while(TRUE)
  {
    FD_ZERO(&readfds);
    
    FD_SET(exfd,&readfds);
    max_sd = exfd;

    for(i=0;i<MAX_NEIGHBORS;i++)
    {
      sd = client_sockets[i];

      if(sd > 0)
        FD_SET(sd,&readfds);

      if(sd > max_sd)
        max_sd = sd;
    }

    activity = select(max_sd + 1 , &readfds , NULL , NULL , NULL);
    if(activity < 0)
    {
      if(errno == EINTR)
        continue;
      perror("\nNODE SERVER: SELECT FAIL");
      return FAILURE;
    }

    if(FD_ISSET(exfd,&readfds))
    {
      acceptfd = accept(exfd,(struct sockaddr *)&cliaddr,&clilen);
      if(acceptfd < 0)
      {
        printf("\nNODE SERVER ACCEPT FAILED\n");
        return NULL;
      }

      for(i=0;i<MAX_NEIGHBORS;i++)
      {
        if(client_sockets[i] == 0)
        {
          client_sockets[i] = acceptfd;
          break;
        }
      }
    }
 
    for(i=0;i<MAX_NEIGHBORS;i++)
    {
      sd = client_sockets[i];

      if(FD_ISSET(sd,&readfds))
      {
        if(nodeRecv(sd,&mssg) == FAILURE)
        {
          perror("\nNODE SERVER: ERROR RECV\n");
          return NULL;
        }
        else
        {
          printf("\nNODE SERVER: RECEIVED %d\n",mssg.data);
          for(j=0;j<MAX_COMMANDS;j++)
          {
            if(mssg.data == cmd_id[j])
             (*cmd_handler[j])(client_sockets,&mssg);
          }           
        }
      }
    }
  }

  return NULL;

}
