/********************************************************************************
            How to Use

    1.
        SimpleWebServer$ make

        SimpleWebServer$ ./main
    
    2.  Use nc as the client

        $ echo -e "GET / HTTP/1.1\r\nConnection: close\r\n\r\n" | nc localhost 8080

        HTTP/1.1 200 OK
        Server: Apache
        Connection: close
        Content-Type: text/html
        Content-Length: 134

        <html>
        <head>
        <meta http-equiv="refresh" content="1"></head>
        <body>
        <h1> Hello. Your ticket number is 00000015. </h1>
        </body>
        </html>


    3.  Open a browser as the client 

        http://localhost:8080

                                                            COMP9024

 ***********************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> //htons()
#include <string.h>    //memset()
#include <fcntl.h>     //fcntl()
#include <pthread.h>
#include <sys/time.h>
#include <signal.h>
#include "Stack.h"

#define SERVER_PORT     8080
//#define MAX_PENDING     10
#define BUFFER_SIZE     1024

// mutual exclusive
static pthread_mutex_t counterMutex = PTHREAD_MUTEX_INITIALIZER;
// ticket counter
static long counter = 0;

// mutual exclusive
static pthread_mutex_t stackMutex = PTHREAD_MUTEX_INITIALIZER;

struct Stack *pStack;

static void write_one_line(int client_sock, char *msg) {
    write(client_sock, msg, strlen(msg));
    write(client_sock, "\r\n", 2);
}


static void do_error(const char *msg, ...) {
    perror(msg);
    if (pStack) {
        ReleaseStack(pStack);
    }
    exit(-1);
}

#define CONTENT_LENGTH_HEADER  128

/*
    Chrome will send two HTTP requests:
    
    Favorite icon 

        GET /favicon.ico HTTP/1.1

    Default URL

        GET / HTTP/1.1

    We only handle the default URL in this simple web server.
 */
static void *run(void *arg) {
    int client_sock = (int) ((long) arg);
    char lengthHeader[CONTENT_LENGTH_HEADER];
    char responseBuf[BUFFER_SIZE];
    char inputBuf[BUFFER_SIZE];
    char *defaultRequest = "GET / HTTP/1.1";

    memset(inputBuf, 0, BUFFER_SIZE);
    read(client_sock, inputBuf, BUFFER_SIZE);
    int isDefaultURL = 0;
    if (strncmp(inputBuf, defaultRequest, strlen(defaultRequest)) == 0) {
        isDefaultURL = 1;
    } 
    
    long ticketNum = 0;

    // 1. HTTP response line
    if (isDefaultURL) {
        write_one_line(client_sock, "HTTP/1.1 200 OK");
        pthread_mutex_lock(&counterMutex);
        counter++;
        ticketNum = counter;
        pthread_mutex_unlock(&counterMutex); 
    } else {
        write_one_line(client_sock, "HTTP/1.1 404 Not Found");
    }

    // 2. HTTP response headers
    write_one_line(client_sock, "Server: Apache");
    write_one_line(client_sock, "Connection: close");
    write_one_line(client_sock, "Content-Type: text/html");
    if (isDefaultURL) {
        snprintf(responseBuf, BUFFER_SIZE, "<html>\n"
                    "<head>\n"
                    "<meta http-equiv=\"refresh\" content=\"1\">"
                    "</head>\n"
                    "<body>\n"
                    "<h1> Hello. Your ticket number is %08ld. </h1>\n"
                    "</body>\n"
                    "</html>\n", ticketNum);
    } else {
        snprintf(responseBuf, BUFFER_SIZE, "<html>\n"
                    "<head>\n"
                    "</head>\n"
                    "<body>\n"
                    "<h1> 404 Not Found </h1>\n"
                    "</body>\n"
                    "</html>\n");
    }
    snprintf(lengthHeader, CONTENT_LENGTH_HEADER, "Content-Length: %d", (int) strlen(responseBuf));
    write_one_line(client_sock, lengthHeader);
    write(client_sock, "\r\n", 2);

    // 3. HTTP response body
    write_one_line(client_sock, responseBuf);
    // close the socket
    close(client_sock);

    pthread_t tid = pthread_self();

    pthread_mutex_lock(&stackMutex);    
#if 0
    // Output the HTTP request from the browser
    printf("%s\n", inputBuf);
#endif
    StackPush(pStack, tid);
    pthread_mutex_unlock(&stackMutex);

    return NULL;
}

int main(void) {
    int serv_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    // ignore the SIGPIPE signal
    signal(SIGPIPE, SIG_IGN);

    // create a data stack to store the thread IDs of the threads that have finished
    pStack = CreateStack();

    if ((serv_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        do_error("socket() failed");
    }
    
    /*
    SO_REUSEADDR

        https://stackoverflow.com/questions/3229860/what-is-the-meaning-of-so-reuseaddr-setsockopt-option-linux
     */
    int enabled = 1;
    if ( setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, &enabled, sizeof(int)) < 0) {
        do_error("setsockopt() failed");
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(SERVER_PORT);

    if (bind(serv_sock, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        do_error("bind() failed");
    }

    if (listen(serv_sock, SOMAXCONN) < 0) {
        do_error("listen() failed");
    }

    unsigned int addr_len = sizeof(client_addr);
    while (1) {

        client_sock = accept(serv_sock, (struct sockaddr *)&client_addr, &addr_len);
        if (client_sock < 0) {
            perror("accept() failed");
            continue;
        }
        // Handle the HTTP request in a worker thread
        pthread_t worker;
        if (pthread_create(&worker, NULL, run, (void *) ((long) client_sock)) != 0) {
            do_error("pthread_create() failed");
        }
        // call pthread_join() to do some clean-up operations for the threads that have finished
        pthread_mutex_lock(&stackMutex);
        while (!StackIsEmpty(pStack)) {
            pthread_t tid = StackPop(pStack);
            pthread_join(tid, NULL);
        }
#if 0
        char client_name[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, client_name, sizeof(client_name));
        printf("Hello: %s/%d\n", client_name, ntohs(client_addr.sin_port));
#endif
        pthread_mutex_unlock(&stackMutex);
    }

    ReleaseStack(pStack);
    return 0;
}



