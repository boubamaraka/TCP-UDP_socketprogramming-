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
#include <pthread.h>

int portnum = 32345;
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
void * thread_load(void *Psock){
  int sock_dest = 0;
  int size ;
  int num_bytes = 0;
  char * buff = NULL;


  sock_dest = *(int *)Psock;

  if(recv(sock_dest , &size , sizeof(int),0) > 0)
  {

       size = ntohl(size);
       printf("%d:size = %d\n",sock_dest,size);
       buff = (char *) malloc(size);
       memset(buff , 1, size);
       if((num_bytes = send(sock_dest , buff , size,0)) <= 0)
       {
         perror("sending to server error:");


       }
       printf("number of bytes = %d\n", num_bytes);
       size = 0;
       free(buff);
       buff = NULL;
  }


}


int newserver(){
  struct sockaddr_in server;
  int sock,listensock;
 socklen_t size,len;
 struct sockaddr_in server1,cliaddr;

len=sizeof(server1);
char address[100]={0};
fd_set set1 ;
fd_set set2 ;
struct timeval tv;
int  maxfd;
char * buffer2 = NULL;
char buff[50];
int bytes=0;;
int i=0 ;
int check=0;
char buffer[100];
pthread_t treadload=0;
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

      if(send(mainsock , buffer , strlen(buffer),0) > 0)
      {
        printf("buffer  = %s \n", buffer);


                             printf("for loop start  \n");
                               size = sizeof(cliaddr);
      while(1){
         printf("while start %d \n",__LINE__);
        if((listensock = accept(sock, (struct sockaddr *)&cliaddr, &size))){
        printf("connection accepted");

        if(pthread_create(&treadload , NULL ,thread_load,(void *) &listensock) < 0)
        {
             perror("Thread error:");
             return 1;
        }

      }
    else{
        perror("accept failed");
        return 1;
      }
    }
       printf("while just end %d \n",__LINE__);


      }
      close(sock);

return 1;
}






int main(){


  struct sockaddr_in server;


  char s_Id[]="464468\n";
  char task[]="3.1-load\n";
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
