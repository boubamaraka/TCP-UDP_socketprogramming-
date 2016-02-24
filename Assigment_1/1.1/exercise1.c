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


  char task[]="1.1-simple\n";
  char buffer[200];
  int len;

  //convert Student Id to Ascii code


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
        //send Id to server                              //send Id to server

       if(((send(sock, s_Id, strlen(s_Id), 0))>0) &&  ((send(sock, task, strlen(task), 0))>0)){
            //read string that the  server sent
           if((len=recv(sock, buffer, 1024, 0))>0){
             buffer[len]='\0';
             printf(" %s",buffer);
           }
     }



       close(sock);



  return 0;



}
