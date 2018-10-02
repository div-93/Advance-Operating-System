#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/select.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<errno.h>
#include<sys/time.h>
#include<signal.h>


#define MAX_CLIENTS 3



void timer_handler (int signum)
{
 printf ("timer expired\n");
}



int main()
{

   int listenfd,acceptfd;
   int recvbuf;
   int i,sd,client_sockets[3];
   char str[512];
   int enablesock =1;
   int activity, socket_count=0,max_sd;
   fd_set readfds;
   struct sockaddr_in servaddr,cliaddr;
   socklen_t clilen;
     struct sigaction sa;
  struct itimerval timer;


   for(i=0;i<3;i++)
    client_sockets[i] = 0;  


  listenfd = socket(AF_INET,SOCK_STREAM,0);

    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &enablesock, sizeof(int)) < 0)
    printf("\nS:SETSOCKOPT FAIL");
 
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(1300);

  if(inet_pton(AF_INET,"127.0.0.1",&servaddr.sin_addr)<=0)
  {
    printf("\nS:INET_PTON FAIL");
    perror("");
    return 0;
  }
 
 
  if ( (bind(listenfd,(struct sockaddr *)&servaddr, sizeof(servaddr)))<0)
  {
    perror("BIND FAIL ");
    return 0;
  }
 
  if( (listen(listenfd,4))<0)
  { 
    printf("FAIL listen");  
    return 0;
  }
 
  
  while(1)
  {

    FD_ZERO(&readfds);

    FD_SET(listenfd,&readfds);
    max_sd = listenfd;


    for(i=0;i<MAX_CLIENTS;i++)
    {
      sd = client_sockets[i];

      if(sd > 0)
      {
        //socket_count = socket_count + 1;
        //FD_SET(sd,&readfds);
      }

      if(sd > max_sd)
        max_sd = sd;
    }
   

    if(socket_count == 1)
    {
      printf("\nsocket_count = %d",socket_count);
       break;
    }

    activity = select(max_sd + 1 , &readfds , NULL , NULL , NULL);
    if(activity < 0)
    {
      if(errno == EINTR)
        continue;
      perror("SELECT FAIL--1");
     return 0;
    }


    if(FD_ISSET(listenfd,&readfds))
    {

      clilen = sizeof(cliaddr);
      acceptfd = accept(listenfd,(struct sockaddr *)&cliaddr,&(clilen));
     socket_count = socket_count + 1; 
      printf("\nacceptfd = %d",acceptfd);
      if(acceptfd < 0)
      {
        perror("ACCEPT FAILED");
        return 0;
      }

      for(i=0;i<MAX_CLIENTS;i++)
      {
        if(client_sockets[i] == 0)
        {
          client_sockets[i] = acceptfd;
          break;
        }
      }
    }
   
  }



    memset (&sa, 0, sizeof (sa));
    sa.sa_handler = &timer_handler;
    sigaction (SIGALRM, &sa, NULL);
    timer.it_value.tv_sec = 3;
    timer.it_value.tv_usec = 0;
    timer.it_interval.tv_sec = 3;
    timer.it_interval.tv_usec = 0;
    setitimer (ITIMER_REAL, &timer, NULL);



  while(1)
  {

        FD_ZERO(&readfds);

     for(i=0;i<MAX_CLIENTS;i++)
     {
       sd = client_sockets[i];

       if(sd > 0)
       {
         socket_count = socket_count + 1;
         FD_SET(sd,&readfds);
       }

       if(sd > max_sd)
         max_sd = sd;
     }

     activity = select(max_sd + 1 , &readfds , NULL , NULL , NULL);
     if(activity < 0)
     {
       if(errno == EINTR)
         continue;
       perror("SELECT FAIL--2");
       return 0;
     }
 
 for(i=0;i<MAX_CLIENTS;i++)
  {
    sd = client_sockets[i];
   if(FD_ISSET(sd,&readfds))
   {

    recvbuf = recv(sd,str,512,0);
    
    if(recvbuf == -1)
    {
      perror("ERROR RECV: ");
      return 0;
    }
    str[recvbuf] = '\0';
    if(!strcmp(str,"exit"))
    {
      printf("\nClient says exit\n");
      return 0;
    }
    printf("\nrecvbuf = %d",recvbuf);
    printf("\n%s\n",str);
  }
  }
  sleep(1);
  }


  return 0;
}
