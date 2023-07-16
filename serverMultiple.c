#include<stdio.h>
#include<string.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<netinet/in.h>
void handleClient(int );
void error(const char *msg){
    perror(msg);//present in stderr
    exit(1);
}
int main(int argc, char *argv[]){
    if(argc!=2)          //checks if no port number is given
    {
        fprintf(stderr,"Port Number is not provided\n");
        exit(1);
    }
    int sockfd, newsockfd, portNo, n;
    
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen;
    sockfd = socket(AF_INET, SOCK_STREAM, 0); //0 is used for TCP connection
    if(sockfd<0){
        error("Error in opening the socket\n");
        exit(1);
    }
    bzero((char *)&serv_addr, sizeof(serv_addr));
    portNo = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portNo);
    if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))<0){
        error("Binding error\n");
    }
    int temp = listen(sockfd,5);
    if(temp<0){
        error("Listen() function failed\n");
    }
    for(;;){
        clilen = sizeof(cli_addr);
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if(newsockfd<0){
        error("Error on the Accept\n");
        }
        printf("handling client %s\n",inet_ntoa(cli_addr.sin_addr));
        handleClient(newsockfd);
    }
    return 0;
}
void handleClient(int newsockfd){
    char buffer[255];
    int recvMsgSize;
    bzero(buffer,250);
    recvMsgSize = read(newsockfd, buffer, 255);
    if(recvMsgSize<0){
        error("Error on reading\n");
    }
    while(1){
        printf("Client: %s",buffer);
        bzero(buffer,255);
        fgets(buffer,255,stdin);
        recvMsgSize = send(newsockfd, buffer, strlen(buffer),0);
        if(recvMsgSize<0){
            error("Error on writing\n");
        }
        bzero(buffer,250);
        recvMsgSize = recv(newsockfd, buffer, 255,0);
        if(recvMsgSize<0){
            error("Error on reading\n");
        }
        
    //    buffer[bytes_read]='\0';

        
     //   n = write(newsockfd, buffer, strlen(buffer));
        
        int i = strncmp("Bye",buffer,3);
        if(i==0)
        break;
    }
    close(newsockfd);
    //close(newsockfd);

}
