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
  char task[]="2.3-server\n";
  char buffer[100];
//  int len;
  int sock2,listensock;
  struct sockaddr_in servaddr,cliaddr,servaddr1;
  char address[100]={0};
  socklen_t size = 0;
  socklen_t len;
  int port=56400;
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
          while(1){
            memset(buffer, 0, sizeof(buffer));
           if(recv(sock, buffer, 1024, 0)>0){
             //buffer[len]='\0';
              printf(" %s",buffer);}
            if(strcmp(buffer,"OK\n")==0){
              break;
            }
            if(strcmp(buffer,"MORE\n")==0){
                printf("buffer = %s",buffer);
              printf("-------------creating socket------------------\n");

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
                   printf("----------send successfull------------------\n");

                  }
                  len=sizeof(cliaddr);
                  printf("line buffer accepting %d \n",__LINE__);
                  while(1){
                  if ((listensock = accept(sock2, (struct sockaddr *) &cliaddr, &len)) < 0) {
                           perror("accept");
                           continue;
                        }
                        while(1){
                        int val;
                    if((val=recv(listensock,&bytes,sizeof(bytes),0))<0){
                         perror("accept");
                         return -1;

                       }


                       printf("Bytes to send is: %u\n", (bytes));
                       if (val==0){
                         printf("Connection Closed\n");
                         close(listensock);
                         break;
                       }
                       bytes=ntohl(bytes);
                       char buff[bytes];
                       memset(buff,0,sizeof(buff));
                       while(bit!=bytes){
                         if ( (val= write(listensock, buff, bytes)) < 0 ){
                           perror("write error");
                           return 1;
                         }
                         bit += val;
                       }
                       printf("Bytes sent is: %u\n", (bit));
                       bit = 0;

                       printf("buffer = %s",buffer);
                       printf("line %d \n",__LINE__);

                     }
                     close(sock2);
                     break;

          }

            }
            port++;



           }



      }
       close(sock);



  return 0;


}
