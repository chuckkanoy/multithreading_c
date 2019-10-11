#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>		//netdb.h has hostent



void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server; //struct of type hostent (host entity)

    char buffer[256];
    if (argc < 3)
    {
        fprintf(stderr,"usage %s hostname port\n", argv[0]); //writes error message to standard error file
        exit(0);
    }

    //CREATE SOCKET
    portno = atoi(argv[2]); //converts argv[2] to an integer
    sockfd = socket(AF_INET, SOCK_STREAM, 0); //creates endpoint for communication and returns Internet socket descriptor
    if (sockfd < 0)
        error("ERROR opening socket");

    //CONNECT TO SERVER
    server = gethostbyname(argv[1]); //get IP address of server
    if (server == NULL)
    {
        fprintf(stderr,"ERROR, no such host\n"); //print error message if server has no value
        exit(0);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr)); //initializes server address to zero
    serv_addr.sin_family = AF_INET; //ALWAYS set to AF_INET
    bcopy((char *)server->h_addr,
          (char *)&serv_addr.sin_addr.s_addr,
          server->h_length); //copies h_addr to s_addr
    serv_addr.sin_port = htons(portno);

    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
        error("ERROR connecting"); //produce error message if error code exists (0 = success, -1 = failure)

    //LOOP
    while(strcmp(buffer,"EXIT\n") != 0)
    {
        printf("Please enter the message: ");
        bzero(buffer,256);
        fgets(buffer,255,stdin); //read input from user to send to server
        //if (buffer is "EXIT") close(sockfd)
        if(strcmp(buffer, "EXIT\n") != 0){
            n = write(sockfd,buffer,strlen(buffer)); //write to socket
            if (n < 0)
                error("ERROR writing to socket"); //produce error message if error code exists
            bzero(buffer,256); //buffer is repeatedly returned to zero for future functions
            n = read(sockfd,buffer,255);
            if (n < 0)
                error("ERROR reading from socket");
            printf("%s\n",buffer);
        }
    }
    //END LOOP

    close(sockfd); //NOTE: experiment with getting rid of this and only having loop close
    return 0; //close everything up and end function; should happen after looping is complete
}
