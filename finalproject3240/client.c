#include "csapp.h"
#include <arpa/inet.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char *argv[]){
    
    int clientfd;  //file descriptor to communicate with the server
    char *host, *port;
    size_t n;

    if (argc != 3)
    {
        fprintf(stderr, "usage: %s <host> <port>\n", argv[0]);
	   exit(0);
    }

    host = argv[1];
    port = argv[2];

    clientfd = Open_clientfd(host, port); //wrapper function that calls getadderinfo, socket and connect functions for client side
    //while loop to keep prompting menu until user wants to terminate



    int terminate = 0;
    char keyInput[4];
    char instrumentChoice[3];
    char buffer[MAXLINE];

    printf("//////////////////////////////////////////////////\n");
    printf("//////////// GUITAR PRACTICE MAKER ///////////////\n");
    printf("//////////////////////////////////////////////////\n");
    printf("\t\ttype Q to quit.\n\n");
    while (terminate==0){
        printf("Enter a Musical Key [A,A#,B,C,D,D#,E,F,F#,G,G#]: ");

        
        bzero(keyInput,4);
        Fgets(keyInput,4,stdin);
        keyInput[strlen(keyInput)-1] = '\0';
        n = write(clientfd,keyInput,strlen(keyInput));
        

if(strcmp(keyInput,"A")==0||strcmp(keyInput,"A#")==0||strcmp(keyInput,"B")==0||strcmp(keyInput,"C")==0||strcmp(keyInput,"C#")==0||strcmp(keyInput,"D")==0||strcmp(keyInput,"D#")==0||strcmp(keyInput,"E")==0||strcmp(keyInput,"F")==0||strcmp(keyInput,"F#")==0||strcmp(keyInput,"G")==0||strcmp(keyInput,"G#")==0 ){
    

        printf("Enter Instrument of Choice, Guitar or Bass? [G,B]: ");

            
            bzero(instrumentChoice,3);
            Fgets(instrumentChoice,3,stdin);
            instrumentChoice[strlen(instrumentChoice)-1] = '\0';
            n = write(clientfd,instrumentChoice,strlen(instrumentChoice));
    
            if(strcmp(instrumentChoice,"G")==0 || strcmp(instrumentChoice,"B")==0  ){
                
                printf("Selected Key is: %s\n", keyInput);
                printf("Your Tab is loading... \n");


                //RECIEVE INPUT
                if(strcmp(instrumentChoice,"G")==0){
                    sleep(1);
                     //resetting the buffer
                bzero(buffer,MAXLINE);
                n = read(clientfd,buffer,MAXLINE);
                //displaying the message in buffer on the console
                Fputs(buffer,stdout);

                if(n<0){
                    perror("Read Error!!");
                    return(-1);
                }
                    

                    printf("\n");
                } else if (strcmp(instrumentChoice,"B")==0){
                    sleep(1);
                    bzero(buffer,MAXLINE);
                    n = read(clientfd,buffer,MAXLINE);
                    //displaying the message in buffer on the console
                    Fputs(buffer,stdout);
                    if(n<0){
                    perror("Read Error!!");
                    return(-1);
                }
                      
                    
                }
                
            } else if(strcmp(instrumentChoice,"Q")==0){
                printf("quitting!\n");
                terminate=1;
            } else {
                printf("Input not recognized, try again.\n");
            }
        
            if(n<0){
                perror("Write Error!!");
                return(-1);
            }

        } else if(strcmp(keyInput,"Q")==0){
            printf("quitting!\n");
            terminate=1;
        } else {
            printf("Input not recognized, try again.\n");
        }
    }
}