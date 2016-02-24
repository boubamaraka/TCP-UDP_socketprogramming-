#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>

int portnum = 52345;
int mainsock = -1;
int checkconnection(){

   struct sockaddr_in connect1 = {0};
   int port = 5000;
   int Psock = 0;


       if((Psock = socket(AF_INET , SOCK_STREAM , 0)) < 0)
       {
      perror("sockets");
         return -1;
       }

       memset(&connect1 , 0 , sizeof(connect1));

       connect1.sin_family = AF_INET;
       connect1.sin_port = htons(port);
       connect1.sin_addr.s_addr = inet_addr("195.148.124.76");
       bzero(&connect1.sin_zero,8);

    if(connect(Psock , (struct sockaddr *)&connect1 , sizeof(struct sockaddr_in)) < 0)
    {

          perror("connect");
          return -1;
    }

   return Psock;



}
int newserver(){
  struct sockaddr_in server;
  int sock,listensock;
 socklen_t size,len;
 struct sockaddr_in server1,cliaddr;

len=sizeof(server1);
char address[100]={0};
fd_set readset ;
fd_set copyset ;
struct timeval tv;
int  maxfd;
char * data = NULL;
char buff[50];
int bytes=0;;
int i=0 ;
int check=0;
char buffer[100];

  if ((sock=socket(AF_INET, SOCK_STREAM,0))==-1){
    perror("socket: ");
    exit(-1);
  }
  if (getsockname(mainsock, (struct sockaddr *) &server1, &len) < 0){
  perror("getsockname");
  return 1;
  }
  server.sin_family=AF_INET;
  server.sin_port=htons(portnum);
  server.sin_addr = server1.sin_addr;

  if (bind(sock, (struct sockaddr *) &server, sizeof(struct sockaddr_in)) < 0) {
      perror("bind");
      return -1;
  }
 printf("line listen %d \n",__LINE__);
  if (listen(sock, 2) < 0) {
        perror("listen");
        return 1;
    }
    if (!inet_ntop(AF_INET, &server1.sin_addr, address, sizeof(server1)))
    {
      perror("inet_ntop");
      return 1;
    }
    memset(buffer,0,sizeof(buffer));
     printf("line buffer printing %d \n",__LINE__);
    snprintf(buffer, 100, "SERV %s %d\n", address , portnum);
    printf("buffer=%s \n",buffer);
      printf("line buffer sending %d \n",__LINE__);
      if(write(mainsock , buffer , strlen(buffer)) > 0)
      {
        printf("buffer  = %s \n", buffer);

          FD_ZERO(&readset);
          FD_SET(sock , &readset);
                             copyset = readset;
          maxfd = sock + 1;

                              tv.tv_sec = 1;
                              tv.tv_usec = 0;
                             printf("before select \n");
                               size = sizeof(cliaddr);
      for(; ;){
                            readset = copyset;
          if(select(maxfd , &readset , NULL ,NULL , NULL)  >= 0)
                             {
            if(FD_ISSET(sock , &readset))

                                     {
              if((listensock = accept(sock, (struct sockaddr *)&cliaddr, &size)) < 0){
                                             perror("accept");
                                             return -1;
                                             }

                                             if(maxfd < listensock){
                                              maxfd = listensock+1;
                           FD_SET(listensock,&copyset);
                                             }
                                             check= 1;
                                             printf("New connection selected %d\n", listensock);
                                     }
                                     i = 0;
                                while( i < maxfd)
            {
              if((FD_ISSET(i,&readset)) && (i != sock) )
              {
                                                printf("sock %d selected\n ",i);
                                                listensock = i;
                                               check= 1;
                                             }
                                             i++;
                                     }

                                     if(check == 1)
                                     {
                                        check= 0;
                                         bytes = read(listensock , &size , sizeof(int));
                memset(buff , 0 , sizeof(buff));
                snprintf(buff , 4 , "%d" , size);
                                         printf("buff = %s\n",buff);
                size = ntohl(size);
                printf("size = %d %d\n", size, bytes);
                                         data = (char *) malloc (size);
                memset(data , 1 , size);
                data[size] = '\0';
                bytes = write(listensock , data , size);
                printf("Bytes = %d\n", bytes);
                free(data);
                data = NULL;


                                     }
                             }
                             else
                             {
                                 perror("select");
                             }
       }

      }
      close(sock);

return 1;
}






int main(){


  struct sockaddr_in server;


  char s_Id[]="464468\n";
  char task[]="2.4-multi\n";
  int returnval;
  char buffer[100];
  returnval=checkconnection();
  if(returnval<0){
    printf("failed to connect \n");
    return 1;
  }
  mainsock=returnval;
   if(((send(mainsock, s_Id, strlen(s_Id), 0))>0) &&  ((send(mainsock, task, strlen(task), 0))>0)){
     printf("successfully sent your information to the server \n");
     returnval = newserver();

     while(read(mainsock , buffer, sizeof(buffer)) > 0)
     {


           printf("Received  = %s\n", buffer);
           bzero(&server,sizeof(server));
           memset(buffer, 0 , sizeof(buffer));

     }

   }
  return 1;
}
