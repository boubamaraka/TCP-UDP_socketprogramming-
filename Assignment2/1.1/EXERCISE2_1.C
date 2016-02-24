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

int connect(char  * buffer)
{
  char type[10] ;
  char addr[100] ;
  char port[1];
  struct sockaddr_in Ipv4client;
  struct sockaddr_in6 Ipv6client;

  int portnum;
  char address[100] = {0};
  char out_buffer[100] = {0};


 int sock2;
  const char s[2] = " ";
  char *token;
  token = strtok(buffer, s);
  int i=0;

  socklen_t size = 0;


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
 printf("Address= %s \n",addr);
 printf("Port= %s \n",port);
 sscanf(port, "%d", &portnum);
 printf("--------Testing if IPV4 or IV6 ------\n");
 printf("%d \n",portnum);
if(strcmp("CONN",type)!=0){
  return 1;
}
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


    size = sizeof(struct sockaddr_in);

    if (connect(sock2,(struct sockaddr *)&Ipv4client,sizeof( Ipv4client)) == -1) {
           perror("connection error occur");
           exit(-1) ;
       }

     if (getsockname(sock2, (struct sockaddr *) &Ipv4client, &size) < 0){
		perror("getsockname");
		return -1;
	}


    if (!inet_ntop(AF_INET, & Ipv4client.sin_addr,  address, sizeof( Ipv4client))) {
      perror("inet_ntop");
      return -1;
    }

    printf("my local address is: %s  my local port is: %d\n",
        address, ntohs(Ipv4client.sin_port));
              snprintf(out_buffer, 100, "ADDR %s %d 464468\n", address , ntohs(Ipv4client.sin_port));



 }
 else{
    printf("Welcom------IPV6\n");
    if ((sock2=socket(AF_INET6, SOCK_STREAM,0))==-1){
      perror("socket: ");
      exit(-1);
    }
       printf(" Debug %d  %d\n",__LINE__,sock2);

    Ipv6client.sin6_family=AF_INET6;
    Ipv6client.sin6_port=htons(portnum);
   inet_pton(AF_INET6,addr, &(Ipv6client.sin6_addr));

       printf(" Debug %d\n",__LINE__);

    if (connect(sock2,(struct sockaddr *) &Ipv6client,sizeof( Ipv6client)) == -1) {
           perror("connection error occur");
           exit(-1) ;
       }
       printf(" Debug %d\n",__LINE__);
      //struct sockaddr_in6 localad;
      size = sizeof(Ipv6client);
      if (getsockname(sock2, (struct sockaddr *) &Ipv6client, &size) < 0) {
          perror("getsockname");
          return -1;
       }
    if (!inet_ntop(AF_INET6, & Ipv6client.sin6_addr,  address, 100)) {
      perror("inet_ntop");
      return -1;
    }

    printf("my local address is: %s  my local port is: %d\n",
        address, ntohs(Ipv6client.sin6_port));
              snprintf(out_buffer, 100, "ADDR %s %d 464468\n", address , ntohs(Ipv6client.sin6_port));
 }
 if((send(sock2, out_buffer, strlen(out_buffer), 0))>0){
 printf("successfull sending of your address %s",buffer);
 }


  return 1;
 }



int main(int argc, char *argv[])
{


  int sock;

  struct sockaddr_in server;


  char s_Id[]="464468\n";
  char task[]="2.1-address\n";
  char buffer[100];

  int len;


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



       len = 1;
       if(((send(sock, s_Id, strlen(s_Id), 0))>0) &&  ((send(sock, task, strlen(task), 0))>0)){
        while(len > 0){
           if((len=recv(sock, buffer, 1024, 0))>0){
             buffer[len]='\0';
             printf(" %s",buffer);
           }
           printf("------unpacking the address the server sent -------\n");
           connect(buffer);

           printf("-------local address -----------\n");
           printf("%s",buffer);
           printf("-------sending the address------\n");

         }
         //memset(buffer,0,sizeof(buffer));
           if((len=recv(sock, buffer, 1024, 0))>0){

           printf(" %s",buffer);
           }
           printf("otput %s",buffer);
           if(strcmp("OK\n",buffer)==0){
             close(sock);
             return 0;
           }


        }
       close(sock);



  return 0;


}
