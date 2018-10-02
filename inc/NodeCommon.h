#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<netdb.h>
#include<ifaddrs.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<net/if.h>
#include<sys/ioctl.h>
#include<netinet/in.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<arpa/inet.h>


#define FAILURE 0
#define SUCCESS 1
#define TRUE 1
#define MAX_NEIGHBORS 10
#define CONNECT_PEER 100
#define SEND_HASH 200
#define SEND_TERM 300
#define MAX_COMMANDS 3

struct node_mssg
{
  int internal;
  int external;
  int data;
  char ip[20];
  int port;
  char hash[30];
};
