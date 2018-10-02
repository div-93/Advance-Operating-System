#include<stdio.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<sys/mman.h>
#include <sys/wait.h>
#include<string.h>


int *sockfd;

void connect_api()
{
  int port[2] = {1200,1300};
  int i=0;
  struct sockaddr_in servaddr[2]; 
  
  while(i<2)
  {
   // sockfd[i] = socket(AF_INET,SOCK_STREAM,0);
    servaddr[i].sin_family = AF_INET;
    servaddr[i].sin_port = htons(port[i]);
    
    if(inet_pton(AF_INET,"127.0.0.1",&servaddr[i].sin_addr)<=0)
       printf("\nINET_PTON FAIL\n");
   
    i++;
  }
  
  i=0;
  while(i<2)
  {
    //*sockfd = socket(AF_INET,SOCK_STREAM,0);
    //servaddr.sin_family = AF_INET;
    //servaddr.sin_port = htons(1200);

    //if(inet_pton(AF_INET,"127.0.0.1",&servaddr.sin_addr)<=0)
       //printf("\nINET_PTON FAIL\n");

    if (connect(sockfd[i], (struct sockaddr*)&servaddr[i],sizeof(struct sockaddr_in))<0)
    {
       perror("CONNECT FAIL");
       return ;
    }
    else
    {
      printf("chiled connection established %d",i);
    }
    i++;
  }
  return ;
}

int main()
{
  int i;
  int sendbuf;
  int buflen;
  char buf[512]; 
  pid_t pid,wpid;
  int status;
  
  sockfd = (int *)malloc(2*sizeof(int));

  sockfd = mmap(NULL,2 * sizeof(int), PROT_READ | PROT_WRITE,MAP_SHARED | MAP_ANONYMOUS, -1, 0); 
  
  strcpy(buf,"Hey");

  i = 0;
  while(i<2)
  {
    sockfd[i] = socket(AF_INET,SOCK_STREAM,0);
    i++;
  }

  pid = fork();

  if(pid == 0)
  {
    connect_api();
    return 0;
  }
  else
  {
       do {
      wpid = waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }
  
  for(i=0;i<2;i++)
    printf(" %d",sockfd[i]);
  i=0;
  while(i<2)
  {
    printf("\nparent Enter Message for the server");
    scanf("%s",buf);
  
    sendbuf = send(sockfd[i],buf,sizeof(buf),0);
  
    if( sendbuf < 0)
    {
      perror ("SEND ERROR");
      return 0;
    }
    
    if(!strcmp(buf,"exit"))
      return 0;

    i++;
  }
  
  //printf("\nsendbuf = %d",sendbuf);
  //printf("\nMessage Sent : %s\n",buf);

  return 0;
}
