#include"NodeClnt.h"

void initClSockfds(int * sockfds)
{
  int i;
  
  for(i=0;i<MAX_NEIGHBORS;i++)
    sockfds[i] = 0;
}

void estblshConn(char * ip, unsigned short port, int * sockfds)
{
  int i;
 
  for(i=0;i<MAX_NEIGHBORS;i++)
  {
    if(i == 0)
      break;
  }
 
  sockfds[i] = nodeConnect(ip,port);

  if(sockfds[i] == 0)
    printf("\nNODE CLIENT: CONNECTION TO PEER %s AT PORT %d FAILED\n",ip,(int)port);
  else
    printf("\nNODE CLIENT: CONNECTION TO PEER %s AT PORT %d ESTABLISHED\n",ip,(int)port);
}

void sendPeer(int * sockfds,struct node_mssg * pmssg)
{
  int i;
 
  while(sockfds[i] != 0)
  {
    if(nodeSend(sockfds[i],pmssg) == 0)
    {
      printf("\nNODE CLIENT: SEND TO PEER FAILED\n");
    }
  }
}

void * nodeClnt(void * args)
{
  struct node_mssg mssg;
  int sockfds[MAX_NEIGHBORS];
  int infd;
  unsigned short *temp,in_clnt; 
 
  temp = (unsigned short *)args;
  in_clnt = *temp;
 
  initClSockfds(sockfds);
  
  do
  {
    sleep(3);
    infd = nodeConnect("127.0.0.1",in_clnt);
 
    if(infd == FAILURE)
    {
      printf("\nNODE CLIENT: INTERNAL CONNECT FAILED\n");
      return NULL;
    }
    else
      printf("\nNODE CLIENT: INTERNAL CONNECTION ESTABLISHED\n");

    sleep(1);
  }while(infd == FAILURE);

  while(TRUE)
  {
    if(nodeRecv(infd,&mssg) == SUCCESS)
    {
      switch(mssg.data)
      {
        case CONNECT_PEER: 
        {
          printf("\nNODE CLIENT: CONNECTION REQUEST RECEIVED\n");
          estblshConn(mssg.ip,mssg.port,sockfds);
          break;
        }
        case SEND_HASH:
        {
          sendPeer(sockfds,&mssg);
          break;
        }
      }
    }
    else
      printf("\nNODE CLIENT: RECEIVE FAILED\n");
  }
  return NULL;
}
