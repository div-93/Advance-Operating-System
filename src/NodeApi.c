#include "NodeCommon.h"

void nodeConPeer(int * client_sockets, struct node_mssg * mssg)
{
  if((nodeSend(client_sockets[0],(void *)mssg)) == FAILURE)
  {
    printf("\nNODE API: CONNECION REQUEST TO CLNT THRD FAILED\n");
    return;
  }
  return;
}

void nodeSendHash(int * client_sockets, struct node_mssg * mssg)
{
  if((nodeSend(client_sockets[0],(void *)mssg)) == FAILURE)
  {
    printf("\nNODE API: CONNECION REQUEST TO CLNT THRD FAILED\n");
    return;
  }
  return;
}

void nodeSendTerm(int * client_sockets, struct node_mssg * mssg)
{
  if((nodeSend(client_sockets[0],(void *)mssg)) == FAILURE)
  {
    printf("\nNODE API: CONNECION REQUEST TO CLNT THRD FAILED\n");
    return;
  }
  return;
}

