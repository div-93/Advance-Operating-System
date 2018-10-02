#include "NodeCommon.h"

int nodeConnect(char * ipaddr, unsigned short port)
{
  int sockfd;
  struct sockaddr_in servaddr;
 
  sockfd = socket(AF_INET,SOCK_STREAM,0);
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(port);

  if(inet_pton(AF_INET,ipaddr,&servaddr.sin_addr)<=0)
  {
    printf("\nNODE COMMMON: INET_PTON FAIL FOR IPADDRESS : %s\n",ipaddr);
    return FAILURE;
  }

  if(connect(sockfd, (struct sockaddr*)&servaddr,sizeof(struct sockaddr_in))<0)
  {
     printf("\nNODE COMMON: CONNECT FAIL FOR IPADDRESS: %s, PORT : %d\n",ipaddr,(int)port);
     perror("");
     return FAILURE;
  }

  return sockfd;
}

int nodeSend(int sockfd, struct node_mssg * pmssg)
{
  if(send(sockfd,(void *)pmssg,sizeof(struct node_mssg),0) < 0)
  {  
    printf("\nNODE COMMON:  SEND ERROR:");
    perror("");
    return FAILURE;
  }
  
  return SUCCESS;
}

int nodeRecv(int sockfd, struct node_mssg * pmssg)
{
  if(recv(sockfd,(void *)pmssg,sizeof(struct node_mssg),0) < 0)
  {
    printf("\nNODE COMMON:  RECV ERROR:");
    perror("");
    return FAILURE;
  }
  
  return SUCCESS;
}

int nodeBind(char * ipaddr, int port)
{
  struct sockaddr_in servaddr;
  int listenfd;
  int enablesock = 0;
 
  listenfd = socket(AF_INET,SOCK_STREAM,0);
  
  if(listenfd < 0)
  {
    printf("\nNODE BIND SOCKET CREATION FAIL\n");
    return FAILURE;
  }
 
  if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &enablesock, sizeof(int)) < 0)
    printf("\nNODE BIND SOCKOPT FAIL\n");
   
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(port);

  if(inet_pton(AF_INET,ipaddr,&servaddr.sin_addr)<=0)
  {
    printf("\nNODE BIND INET_PTON FAIL\n");
    perror("");
    return FAILURE;
  }

  if ( (bind(listenfd,(struct sockaddr *)&servaddr, sizeof(struct sockaddr_in)))<0)
  {
    printf("\nNODE BIND FAIL:");
    perror("");
    return FAILURE;
  }
  
  if((listen(listenfd,100))<0)
  {
    printf("\nNODE BIND LISTEN FAIL\n");
    return FAILURE;
  }
  
  return listenfd;
}

  

