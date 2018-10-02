#include "NodeCommon.h"
#include "NodeCli.h"

#define CLI_DELIMIT " \n"
#define CLI_SIZE 1024

int node_connect(char **args);
int node_hi(char **args);
int node_exit(char **args);

char *builtin_str[] = {
  "peer",
  "hi",
  "exit"
};

int (*builtin_func[]) (char **) = {
  &node_connect,
  &node_hi,
  &node_exit
};

int node_connect(char **args)
{
  struct node_mssg mssg;

  mssg.internal = 1;
  mssg.external = 0;
  strcpy(mssg.ip,args[1]);
  mssg.data = CONNECT_PEER;
  mssg.port = atoi(args[2]); 
  //printf("\nNODE CLI: Trying to Send to server");
  if(nodeSend(gsockfd,(void *)&mssg) == FAILURE)
    printf("\nNODE CLI: SEND TO SERVER FAILED\n");
  //else
    //printf("\nNODE CLI: SEND TO SERVER SUCCESS");

  return SUCCESS;
}

int node_hi(char **args)
{
  printf("\nHi\n");
  return SUCCESS;
}

int node_exit(char **args)
{
  return 0;
}

char * nodeReadLine()
{
  char *line  = NULL;
  size_t cli_size = 0;
  
  getline(&line, &cli_size, stdin);
  
  return line;
}

char ** nodeSplitLine(char * line)
{
  char **tokens = malloc(CLI_SIZE * sizeof(char *));
  char *token;
  int positions = 0;

  token = strtok(line,CLI_DELIMIT);
  while(token != NULL)
  {
    tokens[positions] = token;
    positions++; 
    token = strtok(NULL,CLI_DELIMIT);
  }
  return tokens;
}

int nodeLaunch(char ** args)
{
  int pid;
  int wpid,status; 
  pid = fork();
   
  if(pid == 0)
  {
    //execvp(args[0],args);
    if (execvp("./nodeCliLoop.o", args) == -1) 
    {
      perror("\nNODE");
    } 
    //exit(0);
  }
  else
  {
    do 
    {
      wpid = waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }
}

int node_num_builtins() {
  return sizeof(builtin_str) / sizeof(char *);
}

int nodeExecute(char **args)
{
  int i;

  if (args[0] == NULL) {
    // An empty command was entered.
    return 1;
  }

  for (i = 0; i < node_num_builtins(); i++) {
    if (strcmp(args[0], builtin_str[i]) == 0) {
      return (*builtin_func[i])(args);
    }
  }

  //return nodeLaunch(args);
}

void nodeCliLoop(void)
{
  char *line;
  char **args;
  int status;

  do
  {
    printf("NODE>");
    line = nodeReadLine();
    args = nodeSplitLine(line);
    status = nodeExecute(args);
    
    free(line);
    free(args);
  }while(status);
}
 
void connServTh(char * ip, unsigned short port)
{
  do
  {
    sleep(3);
    gsockfd = nodeConnect(ip,port);

    if(gsockfd == FAILURE)
      printf("\nNODE CLI : CONNECTION TO SERV THRD FAILED\n");
    else
      printf("\nNODE CLI : CONNECTION TO SERV THRD ESTABLISHED\n");

    sleep(1);
  }while(gsockfd == FAILURE);
}

void * nodeCli(void * args)
{
  unsigned short *temp;
  unsigned short in_cli;
  temp = (unsigned short *)args;
  in_cli = *temp;
  connServTh("127.0.0.1",in_cli);
  nodeCliLoop();
  return NULL;
}


