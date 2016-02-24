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

int main(int argc, char *argv[])
{


  int sock;

  struct sockaddr_in server;


  char s_Id[]="464468\n";
  char task[]="2.4-multi\n";
  char buffer[100];
//  int len;
  int sock2,listensock;
  struct sockaddr_in servaddr,cliaddr,servaddr1;
  char address[100]={0};
  socklen_t size = 0;
  socklen_t len;
  int port=52110;
  fd_set set_toread ;
	fd_set set_tocopy ;
  struct timeval tv;
  int  maxfd;
  FD_ZERO(&set_toread);    // clear the master and temp sets
  FD_ZERO(&set_tocopy);
  //char buffer[100] = {0};
  //char ip[]="195.148.124.76";



  //socket created here
  if ((sock=socket(AF_INET, SOCK_STREAM,0))==-1){
    perror("socket: ");
    exit(-1);
  }
  memset(&server, 0, sizeof(server));
  server.sin_family=AF_INET;
  server.sin_port=htons(5000);
  server.sin_addr.s_addr = inet_addr("195.148.124.76");
  bzero(&server.sin_zero,8);
  int len2 =sizeof(server);
    uint32_t bytes=0,bit=0;

    //connecting to server
    if (connect(sock,(struct sockaddr *)&server,len2) == -1) {
           perror("connection error occur");
           exit(-1) ;
       }




       if(((send(sock, s_Id, strlen(s_Id), 0))>0) &&  ((send(sock, task, strlen(task), 0))>0)){

            memset(buffer, 0, sizeof(buffer));
           if(recv(sock, buffer, 1024, 0)<0){

              return 1;}
            if(strcmp(buffer,"OK\n")==0){
              return 1;
            }
            if((sock2=socket(AF_INET, SOCK_STREAM,0))==-1){
              perror("socket: ");
              exit(-1);
            }
            len=sizeof(servaddr1);
           size = sizeof(struct sockaddr_in);
           printf("line %d \n",__LINE__);
          if (getsockname(sock, (struct sockaddr *) &servaddr1, &len) < 0){
          perror("getsockname");
          return -1;
          }
           memset(&servaddr, 0, sizeof(servaddr));
          servaddr.sin_family=AF_INET;
          servaddr.sin_port=htons(port);
          servaddr.sin_addr = servaddr1.sin_addr;
          bzero(&servaddr.sin_zero,8);

          printf("line bind %d \n",__LINE__);
           if (bind(sock2, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
               perror("bind");
               return -1;
           }
          printf("line listen %d \n",__LINE__);
           if (listen(sock2, 2) < 0) {
                 perror("listen");
                 return -1;
             }
             printf("line allocating %d \n",__LINE__);
               if (!inet_ntop(AF_INET, &servaddr.sin_addr,  address, sizeof(address))) {
                 perror("inet_ntop");
                 return -1;
               }
               memset(buffer,0,sizeof(buffer));
                printf("line buffer printing %d \n",__LINE__);
               snprintf(buffer, 100, "SERV %s %d\n", address , port);
               printf("buffer=%s \n",buffer);
                 printf("line buffer sending %d \n",__LINE__);

               if((send(sock,buffer,strlen(buffer),0))>0){
               printf("--------succesful sending ---------------\n");
                FD_SET(sock2, &set_toread);
                maxfd=sock2;
                //for(;;){

                printf("line in the main for loop %d \n",__LINE__);
                 set_tocopy=set_toread;
                 if (select(maxfd+1, &set_tocopy, NULL, NULL, NULL) >= 0) {
                  printf("Server selected %d\n",maxfd);
                 }
                 printf("line after selection %d \n",__LINE__);

                 int i=0;


              // }//end for main loop


              }//end if loop for main sockect sending my local address





         close(sock2);



      }//emd if loop for student information sending
       close(sock);



  return 0;


}
