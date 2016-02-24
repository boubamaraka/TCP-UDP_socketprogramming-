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

   char type[10] ;
   char addr[30] ;
   char port[1];


  int sock;
  int sock2;
  struct sockaddr_in server;
  struct sockaddr_in Ipv4client;
  struct sockaddr_in6 Ipv6client;

  char s_Id[]="464468\n";
  char task[]="2.1-address\n";
  char buffer[100];
  char buffer2[100];
  int len;
  int portnum;
  char address[100] = {0};

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

    //connecting to server
    if (connect(sock,(struct sockaddr *)&server,len2) == -1) {
           perror("connection error occur");
           exit(-1) ;
       }




       if(((send(sock, s_Id, strlen(s_Id), 0))>0) &&  ((send(sock, task, strlen(task), 0))>0)){

           if((len=recv(sock, buffer, 1024, 0))>0){
             buffer[len]='\0';
             printf(" %s",buffer);
           }
           printf("------unpacking the address the server sent -------\n");
           const char s[2] = " ";
           char *token;
           token = strtok(buffer, s);
           int i=0;
          while(token!=NULL){
            i++;
            if(i==1)
            strcpy(type,token);
            else if(i==2)
            strcpy(addr,token);
            else if (i==3)
            strcpy(port,token);
            printf( " %s\n", token );

           token = strtok(NULL, s);
          }
          printf("--------address info Printing ------\n");
          printf("type= %s \n",type);
          printf("type= %s \n",addr);
          printf("type= %s \n",port);
          sscanf(port, "%d", &portnum);
          printf("--------Testing if IPV4 or IV6 ------\n");
          printf("%d \n",portnum);

        	if (strlen(addr)<=15) {
             printf("------Welcome -----IPV4---- \n");
             if ((sock2=socket(AF_INET, SOCK_STREAM,0))==-1){
               perror("socket: ");
               exit(-1);
             }
             Ipv4client.sin_family=AF_INET;
             Ipv4client.sin_port=htons(portnum);
             Ipv4client.sin_addr.s_addr = inet_addr(addr);
             bzero(&Ipv4client.sin_zero,8);
             if (connect(sock2,(struct sockaddr *)&Ipv4client,sizeof( Ipv4client)) == -1) {
                    perror("connection error occur");
                    exit(-1) ;
                }

             if (!inet_ntop(AF_INET, & Ipv4client.sin_addr,  address, sizeof( Ipv4client))) {
  				     perror("inet_ntop");
  				     return -1;
  			     }

  			     printf("my local address is: %s  my local port is: %d\n",
  					     address, ntohs(Ipv4client.sin_port));
                       snprintf(buffer, 100, "ADDR %s %d 464468\n", address , ntohs(Ipv4client.sin_port));



          }
        else{
             printf("Welcom------IPV6\n");
             if ((sock2=socket(AF_INET6, SOCK_STREAM,0))==-1){
               perror("socket: ");
               exit(-1);
             }
             Ipv6client.sin6_family=AF_INET6;
             Ipv6client.sin6_port=htons(portnum);
             if (connect(sock2,(struct sockaddr *) &Ipv6client,sizeof( Ipv6client)) == -1) {
                    perror("connection error occur");
                    exit(-1) ;
                }
               struct sockaddr_in6 localad;
               socklen_t size = sizeof(localad);
               if (getsockname(sock2, (struct sockaddr *) &localad, &size) < 0) {
                   perror("getsockname");
                   return -1;
                }
             if (!inet_ntop(AF_INET6, & Ipv6client.sin6_addr,  address, sizeof( Ipv6client))) {
  				     perror("inet_ntop");
  				     return -1;
  			     }

  			     printf("my local address is: %s  my local port is: %d\n",
  					     address, ntohs(Ipv6client.sin6_port));
                       snprintf(buffer, 100, "ADDR %s %d 464468\n", address , ntohs(Ipv6client.sin6_port));
          }
        printf("-------local address -----------\n");
        printf("%s",buffer);
        printf("-------sending the address------\n");

        if((send(sock2, buffer, strlen(buffer), 0))>0){
         printf("successfull sending of your address %s",buffer);
        }
        if((len=recv(sock, buffer2, 1024, 0))>0){

          printf(" %s",buffer2);
        }
        printf("otput %s",buffer2);


     }
       close(sock);



  return 0;



}
