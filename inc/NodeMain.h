#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

#define SUCCESS 1
#define FAILURE 0

void * nodeCli(void*);
void * nodeServ(void *);
void * nodeClnt(void *);
