#include "NodeCommon.h"

void nodeConPeer(int *, struct node_mssg *);
void nodeSendHash(int *, struct node_mssg *);
void nodeSendTerm(int *, struct node_mssg *);

int cmd_id[MAX_COMMANDS]=
{
  CONNECT_PEER,
  SEND_HASH,
  SEND_TERM
};

void (*cmd_handler[MAX_COMMANDS])(int *,struct node_mssg *)=
{
  &nodeConPeer,
  &nodeSendHash,
  &nodeSendTerm
};
