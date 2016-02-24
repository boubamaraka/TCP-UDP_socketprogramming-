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
char * buffer2 = NULL;
char buff[50],ack[10],msg[10];
int bytes=0;
int i=0 ;
int check=0;
char buffer[100];
char datagram_type;
unsigned int datagram_arg,seqnumber=1,lastmessage=1;

  if ((sock=socket(AF_INET, SOCK_DGRAM,0))==-1){
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

    if (!inet_ntop(AF_INET, &server1.sin_addr, address, sizeof(server1)))
    {
      perror("inet_ntop");
      return 1;
    }
    memset(buffer,0,sizeof(buffer));
     printf("line buffer printing %d \n",__LINE__);
    snprintf(buffer, 100, "UDPS %s %d\n", address , portnum);
    printf("buffer=%s \n",buffer);
      printf("line buffer sending %d \n",__LINE__);

      if(send(mainsock , buffer , strlen(buffer),0) > 0)
      {
        printf("buffer  = %s \n", buffer);
         printf("before recvfrom %d \n",__LINE__);
        bytes = recvfrom(sock,buff,sizeof(buff),0,(struct sockaddr *)&cliaddr, &len);
        printf("number of bytes %d:",bytes);
        printf("----------------Datagram sent by the server ------------------\n");
        //printf("Datagram  = %s \n", buff);
        datagram_type=buff[0];
        datagram_arg= ntohl(*(unsigned int *)(buff+1));
         //printf("After receive %d \n",__LINE__);
          printf("datagram_type = %d datagram_arg = %d\n", (int) datagram_type ,datagram_arg);
          getchar();
          if(datagram_arg>0){

            while(seqnumber <= datagram_arg){
              memset(msg, 0, sizeof(msg));
              msg[0]=2;
              for(i=0;i<2; i++)
              {
                      *(unsigned int *)(msg+1) = htonl(seqnumber);
                      bytes = sendto(sock,msg,sizeof(msg),0,(struct sockaddr *)&cliaddr,
                                      sizeof(struct sockaddr_in));
                      seqnumber++;
                      printf("bytes = %d sequence number = %d\n",bytes , seqnumber);
              }
              memset(msg, 0, sizeof(msg));

              if((bytes = recvfrom(sock,msg,sizeof(msg),0,(struct sockaddr *)&cliaddr,
                                               &len)) > 0)
               {
                       if(msg[0] == 3)
                       {
                               lastmessage = ntohl(*(unsigned int *)(msg+1));
                               printf("ack = %d\n", lastmessage);
                               seqnumber = lastmessage+1;
                       }
               }
            }

          }





      }
      close(sock);

return 1;
}






int main(){


  struct sockaddr_in server;


  char s_Id[]="464468\n";
  char task[]="3.2-udp\n";
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
