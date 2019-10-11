#include <stdio.h>
#include <stdlib.h>				//for IOs
#include <string.h>
#include <unistd.h>
#include <sys/types.h>			//for system calls
#include <sys/socket.h>			//for sockets
#include <netinet/in.h>
#include <pthread.h>            //for thread


void error(const char *msg)
{
    perror(msg);
    exit(0);
}

void *handleThread(int mysockfd)
{
    int n;
    char buffer[256]; //lets server read characters from socket connection into buffer
    int sockfd, newsockfd, portno;

    socklen_t clilen;	//stores size of client for system calls
    struct sockaddr_in serv_addr, cli_addr;
    pthread_t pth; //thread identifier

    //LOOP
    while(strcmp(buffer, "EXIT\n") != 0)
    {
        void *ret;
        //sleep(1); //give main enough time to get another client if it needs to

        bzero(buffer,256);
        n = read(mysockfd,buffer,255); //reads from socket
        if (n < 0)
            error("ERROR reading from socket"); //produces error message if error code is present; stops code
        if(strcmp(buffer, "EXIT\n") != 0)
        {
            printf("Here is the message from client %d: %s\n",mysockfd, buffer); //prints message if no error occurs

            n = write(mysockfd,"I got your message",18); //writes message reception message
            if (n < 0)
                {
                    error("ERROR writing to socket"); //produces error message if error code is present
                 }
        }
        else{

            close(mysockfd);
            pthread_exit(NULL);
            //return NULL;
        }

    }
    //END LOOP

    //return NULL;
}

int main(int argc, char* argv[])
{
    int sockfd, newsockfd, portno, pid;

    pthread_t pth; //thread identifier
    socklen_t clilen;	//stores size of client for system calls
    char buffer[256]; //lets server read characters from socket connection into buffer
    struct sockaddr_in serv_addr, cli_addr;

    if (argc < 2)
    {
        fprintf(stderr,"ERROR, no port provided\n");
        exit(1);
    }

    //CREATE SOCKET
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    /*create socket using 3 arguments:
    Internet Protocol v4 address,
    sequenced, two-way, connection-based byte streams,
    0 to allow OS to choose appropriate protocol*/

    if (sockfd < 0)
        error("ERROR opening socket");

    bzero((char *) &serv_addr, sizeof(serv_addr)); //initializes the serv_addr to zeros

    portno = atoi(argv[1]); //set the port to be a string of digits converted to an integer

    serv_addr.sin_family = AF_INET; //ALWAYS set the sin_family of the serv_addr to IPv4 address

    serv_addr.sin_addr.s_addr = INADDR_ANY; //sets the unsigned long of s_addr to whatever address is making connection

    serv_addr.sin_port = htons(portno); //sets sin_port to the host byte order converted to the network byte order

    //BIND SOCKET TO SERVER ADDRESS AND PORT
    if (bind(sockfd, (struct sockaddr *) &serv_addr,
             sizeof(serv_addr)) < 0)
        error("ERROR on binding"); //binds sockaddr to serv_addr, error if this returns -1

    //LISTEN TO SOCKET
    listen(sockfd, 5);
    printf("Listening...\n");
    //LOOP
    while(1)
    {
        //usleep(1); //give main enough time to get another client if it needs to
        clilen = sizeof(cli_addr);
        printf("1\n");
        newsockfd = accept(sockfd,
                           (struct sockaddr *) &cli_addr,
                           &clilen); /*waits until proper connection has been made to a client
                                    then gets a new socket file descriptor */


        if (newsockfd < 0)
            error("ERROR on accept"); //produces error if error code (-1) is present

        /*
        pid = fork();
        if(pid < 0)
            error("ERROR on fork");
        if(pid == 0){
            close(sockfd);
            handleThread(newsockfd);
            exit(0);
        }
        else{
            close(newsockfd);
        }
        */


        //close(sockfd);
        if(pthread_create(&pth, NULL, handleThread, newsockfd) < 0){
           // close(sockfd);
            
            printf("Failed to create thread\n");
        }
        //pthread_join(pth, NULL);
        //usleep(1); //give main enough time to get another client if it needs to

    }
    //END LOOP

    //close(newsockfd);
    close(sockfd);
    return 0; //closes up everything and ends program; loop should complete when all threads are closed
}



