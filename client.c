
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

void *writeToSocket();
void *readFromSocket();

int socket_FD;
struct sockaddr_in server;
char server_reply[2000];


int main(int argc, char *argv[]) {

    // Create socket;
    socket_FD = socket(AF_INET, SOCK_STREAM, 0);

    if(socket_FD == -1) {
        printf("Unable to create socket");
    }
    puts("Socket created!!");
    puts("Enter server ip: ");
    char server_ip[27]; 
    scanf("%s", server_ip);
    server.sin_addr.s_addr = inet_addr(server_ip);
    server.sin_family = AF_INET;
    server.sin_port = htons(55555);
    
    // connect to remote server
    if(connect(socket_FD, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("connect failed. Error");
        return 1;
    }

    puts("Connected\n");

    int  my_id;
    puts("Your id: \n");
    scanf("%i", my_id); 
    // Initial connect

    pthread_t readThread;
    pthread_t writeThread;

    pthread_create(&readThread, NULL, readFromSocket, NULL);
    pthread_create(&writeThread, NULL, writeToSocket, NULL);

    pthread_join(readThread, NULL);

    return 0;
}

void *writeToSocket() {
    while(1) {
        char message[1000];
        printf("Enter message : \n");
        scanf("%s", message);

        // send message
        if( send(socket_FD, message, strlen(message), 0) < 0) {
            puts("send failed");
        }
    }
}


void *readFromSocket() {
    while(1) {
        // recive message
        if(recv(socket_FD, server_reply, 2000, 0) > 0) {
            puts("Server reply: ");
            puts(server_reply);
        }
    }
}

