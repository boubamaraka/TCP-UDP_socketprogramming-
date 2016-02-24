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

#define BUFSIZE 1024

int main(int argc, char *argv[])
{

  int sock;
  struct sockaddr_in server;
  char s_Id[]="464468\n";


  char task[]="1.3-large\n";
  char buffer[15] = {0};
  //int len = 0;
  int Total_len = 0;
  uint32_t buff_size = 0;
  char * buff = NULL;
  int byte=0;


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
            printf("--------step checking the size of object -----------\n");
           while((read(sock,&buff_size, 4))>0){

                if(buff_size == 0){
                  send(sock, &buff_size, sizeof(buff_size),0);
                   read(sock, buffer, BUFSIZE);
                   printf("Result = %s\n", buffer);
                   break;
                }

               buff_size = ntohl(buff_size);

                buff = malloc (buff_size);
              //  memset(buff, 0, buff_size);


               while (Total_len != buff_size){
                  byte = read(sock, buff,buff_size);
                  Total_len += byte;
                }

                buff_size = htonl(buff_size);
                printf("len= %d buffer= %s \n",Total_len,buff);
                //Returning the size.
                if ((send(sock, &buff_size, sizeof(buff_size), 0))<0){
                  exit(-1);
                }
                buff_size=0;
              //  len = 0;
                Total_len = 0;
                free(buff);
                buff=NULL;



           }

     }


       close(sock);

  return 0;



}
