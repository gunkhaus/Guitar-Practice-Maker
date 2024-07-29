//Devin Thompson
//4-27-24
//Guitar Tab Maker that employs the use of threads AND server-client format
#include "csapp.h"
#include <arpa/inet.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
//musical keys
//char keyOfA[7][3]={"A","B","C#","D","E","F#","G#"};
int keyOfA[7]={1,3,5,6,8,10,12};
//char keyOfASharp[7][3]={"A#","C","D","D#","F","G","A"};
int keyOfASharp[7]={2,4,6,7,9,11,1};
//char keyOfB[7][3]={"B","C#","D#","E","F#","G#","A#"};
int keyOfB[7]={3,5,7,8,10,12,2};
//char keyOfC[7][3]={"C","D","E","F","G","A","B"};
int keyOfC[7]={4,6,8,9,11,1,3};
//char keyOfCSharp[7][3]={"C#","D#","F","F#","G#","A#","C"};
int keyOfCSharp[7]={5,7,9,10,12,2,4};
//char keyOfD[7][3]={"D","E","F#","G","A","B","C#"};
int keyOfD[7]={6,8,10,11,1,3,5};
//char keyOfDSharp[7][3]={"D#","F","G","G#","A#","C","D"};
int keyOfDSharp[7]={7,9,11,12,2,4,6};
//char keyOfE[7][3]={"E","F#","G#","A","B","C#","D#"};
int keyOfE[7]={8,10,12,1,3,5,7};
//char keyOfF[7][3]={"F","G","A","A#","C","D","E"};
int keyOfF[7]={9,11,1,2,4,6,8};
//char keyOfFSharp[7][3]={"F#","G#","A#","B","C#","D#","F"};
int keyOfFSharp[7]={10,12,2,3,5,7,9};
//char keyOfG[7][3]={"G","A","B","C","D","E","F#"};
int keyOfG[7]={11,1,3,4,6,8,10};
//char keyOfGSharp[7][3]={"G#","A#","C","C#","D#","F","G"};
int keyOfGSharp[7]={12,2,4,5,7,9,11};


//guitar strings and an octave+1 of notes
//char highE[13][3]={"E","F","F#","G","G#","A","A#","B","C","C#","D","D#","E"};
int highE[13]={8,9,10,11,12,1,2,3,4,5,6,7,8};
//char B[13][3]={"B","C","C#","D","D#","E","F","F#","G","G#","A","A#","B"};
int B[13]={3,4,5,6,7,8,9,10,11,12,1,2,3};
//char G[13][3]={"G","G#","A","A#","B","C","C#","D","D#","E","F","F#","G"};
int G[13]={11,12,1,2,3,4,5,6,7,8,9,10,11};
//char D[13][3]={"D","D#","E","F","F#","G","G#","A","A#","B","C","C#","D"};
int D[13]={6,7,8,9,10,11,12,1,2,3,4,5,6};
//char A[13][3]={"A","A#","B","C","C#","D","D#","E","F","F#","G","G#","A"};
int A[13]={1,2,3,4,5,6,7,8,9,10,11,12};
//char E[13][3]={"E","F","F#","G","G#","A","A#","B","C","C#","D","D#","E"};
int E[13]={8,9,10,11,12,1,2,3,4,5,6,7,8};

//this structure will hold each relevant note degree for the users selection
//each one is 8 long because on a guitar fretboard, from frets 0-12, in any given key, 8 legal notes will be on each string.
typedef struct{
    int highEString[8];
    int BString[8];
    int GString[8];
    int DString[8];
    int AString[8];
    int EString[8];
}UserKey;

typedef struct{
    UserKey myInstance;
    int section;
    int highEAMOUNT;
    int BAMOUNT;
    int GAMOUNT;
    int DAMOUNT;
    int AAMOUNT; 
    int EAMOUNT; 
    char KEY[4];
    int highEcount;
    int Bcount;
    int Gcount;
    int Dcount;
    int Acount;
    int Ecount;
    int highEStart;
    int BStart;
    int GStart;
    int DStart;
    int AStart;
    int EStart;
    int connfd;
}DisplayThreadStuct;

//Function Prototypes
void keyFinder(char keyInput[], UserKey myInstance, char instChoice[], DisplayThreadStuct*myDisp,DisplayThreadStuct*myDisp2,DisplayThreadStuct*myDisp3);
void noteFinder(int chosenKey[7], UserKey myInstance, char instChoice[], DisplayThreadStuct *myDisp, DisplayThreadStuct*myDisp2, DisplayThreadStuct*myDisp3);
void *threadMaker(void*arg);
void *DisplayerGuitar(void*arg );
void *DisplayerGuitar2(void*arg );
void *DisplayerGuitar3(void*arg );
void *DisplayerBass(void*arg);



void* serverFunction(void*arg){

    UserKey myInstance;
    DisplayThreadStuct myDisp;

    myDisp.connfd = *((int *)arg);
    Pthread_detach(pthread_self());
    Free(arg);

    
  //char buffer[MAXLINE]; // MAXLINE = 8192 defined in csapp.h
  // TODO:
  // TODO End
  size_t n;
    

    DisplayThreadStuct *myDispptr = &myDisp;
    DisplayThreadStuct myDisp2;
    DisplayThreadStuct *myDispptr2 = &myDisp2;
    DisplayThreadStuct myDisp3;
    DisplayThreadStuct *myDispptr3 = &myDisp3;
    int terminate = 0;
    char keyInput[4];
    char instrumentChoice[3];


    
    while (terminate==0){
       
        //input check
        // resetting the buffer
    bzero(keyInput, 4);

    n = read(myDisp.connfd, keyInput ,4);
    if (n < 0) {
      perror("Read Error!!");
      return NULL;
    }
    
        
     if(strcmp(keyInput,"A")==0||strcmp(keyInput,"A#")==0||strcmp(keyInput,"B")==0||strcmp(keyInput,"C")==0||strcmp(keyInput,"C#")==0||strcmp(keyInput,"D")==0||strcmp(keyInput,"D#")==0||strcmp(keyInput,"E")==0||strcmp(keyInput,"F")==0||strcmp(keyInput,"F#")==0||strcmp(keyInput,"G")==0||strcmp(keyInput,"G#")==0 ){

            bzero(instrumentChoice, 3);

            n = read(myDisp.connfd, instrumentChoice, 3);
            if (n < 0) {
              perror("Read Error!!");
              return NULL;
            }
            

            
            if(strcmp(instrumentChoice,"G")==0 || strcmp(instrumentChoice,"B")==0  ){
                //send to keyFinder
                keyFinder(keyInput, myInstance, instrumentChoice, myDispptr, myDispptr2 , myDispptr3);
                //REMOVE IN FINAL PRODUCT
                //terminate=1;
            } else if(strcmp(instrumentChoice,"Q")==0){
                printf("quitting!\n");
                terminate=1;
            } else {
                printf("Input not recognized, try again.\n");
            }
        }else if(strcmp(keyInput,"Q")==0){
            printf("quitting!\n");
            terminate=1;
        } else {
            printf("Input not recognized, try again.\n");
        }
    }
    return 0;
}

void keyFinder(char keyInput[], UserKey myInstance, char instChoice[], DisplayThreadStuct *myDisp, DisplayThreadStuct*myDisp2, DisplayThreadStuct*myDisp3){

    //First, we must establish what notes are legal for the given key and on what string.
    if (strcmp(keyInput,"A")==0){
        strcpy(myDisp->KEY,"A");
       noteFinder(keyOfA, myInstance,instChoice, myDisp, myDisp2, myDisp3);

    } else if (strcmp(keyInput,"A#")==0){
        strcpy(myDisp->KEY,"A#");
        noteFinder(keyOfASharp, myInstance,instChoice, myDisp, myDisp2, myDisp3);

    } else if (strcmp(keyInput,"B")==0){
        strcpy(myDisp->KEY,"B");
        noteFinder(keyOfB, myInstance,instChoice,  myDisp, myDisp2, myDisp3);

    } else if (strcmp(keyInput,"C")==0){
        strcpy(myDisp->KEY,"C");
        noteFinder(keyOfC, myInstance,instChoice, myDisp, myDisp2, myDisp3);

    } else if (strcmp(keyInput,"C#")==0){
        strcpy(myDisp->KEY,"C#");
        noteFinder(keyOfCSharp, myInstance,instChoice, myDisp, myDisp2, myDisp3);

    } else if (strcmp(keyInput,"D")==0){
        strcpy(myDisp->KEY,"D");
        noteFinder(keyOfD, myInstance, instChoice, myDisp, myDisp2, myDisp3);

    } else if (strcmp(keyInput,"D#")==0){
        strcpy(myDisp->KEY,"D#");
        noteFinder(keyOfDSharp, myInstance, instChoice,  myDisp, myDisp2, myDisp3);

    } else if (strcmp(keyInput,"E")==0){
        strcpy(myDisp->KEY,"E");
        noteFinder(keyOfE, myInstance, instChoice, myDisp, myDisp2, myDisp3);

    } else if (strcmp(keyInput,"F")==0){
        strcpy(myDisp->KEY,"F");
        noteFinder(keyOfF, myInstance, instChoice, myDisp, myDisp2, myDisp3);

    } else if (strcmp(keyInput,"F#")==0){
        strcpy(myDisp->KEY,"F#");
        noteFinder(keyOfFSharp, myInstance, instChoice, myDisp, myDisp2, myDisp3);

    } else if (strcmp(keyInput,"G")==0){
        strcpy(myDisp->KEY,"G");
        noteFinder(keyOfG, myInstance, instChoice, myDisp, myDisp2, myDisp3);

    } else if (strcmp(keyInput,"G#")==0){
        strcpy(myDisp->KEY,"G#");
        noteFinder(keyOfGSharp, myInstance, instChoice, myDisp, myDisp2, myDisp3);

    } else {
        printf("Something went wrong.\n");
        return;
    }
}

void noteFinder(int chosenKey[7], UserKey myInstance, char instChoice[], DisplayThreadStuct *myDisp, DisplayThreadStuct*myDisp2, DisplayThreadStuct*myDisp3){
    int place = 0;
    int temp;
    int highEAMOUNT=0;
    int BAMOUNT=0;
    int GAMOUNT=0;
    int DAMOUNT=0;
    int AAMOUNT=0;
    int EAMOUNT=0;

    //use nested for loops to take each element of the chosen key array and each element of a given 6 string and find the index of the legal notes

        //for each note in the key...
    for (int i = 0 ; i < 7 ; i++){
        //for each fret...
        for (int j = 0; j < 13 ; j++){
            //if the key and place on fretboard lines up
            if (chosenKey[i]==highE[j]){
                myInstance.highEString[place]=j;
                //printf("%i : %i, %i \n",myInstance.highEString[place], i , j);
                place++;
                highEAMOUNT++;
            }
        }
    }
    //reorganize from smallest num to greatest (moving down the fretboard)
    for(int i = 0 ; i < highEAMOUNT ; i++){
        for (int j = 0 ; j < highEAMOUNT ; j++){
            if (myInstance.highEString[i]<myInstance.highEString[j]){
                temp = myInstance.highEString[j];
                myInstance.highEString[j]=myInstance.highEString[i];
                myInstance.highEString[i]=temp;
            }
        }
    }
    place=0;

   //for each note in the key...
    for (int i = 0 ; i < 7 ; i++){
        //for each fret...
        for (int j = 0; j < 13 ; j++){
            //if the key and place on fretboard lines up
            if (chosenKey[i]==B[j]){
                myInstance.BString[place]=j;
                place++;
                BAMOUNT++;
            }
        }
    }
    //reorganize from smallest num to greatest (moving down the fretboard)
    for(int i = 0 ; i < BAMOUNT ; i++){
        for (int j = 0 ; j < BAMOUNT ; j++){
            if (myInstance.BString[i]<myInstance.BString[j]){
                temp = myInstance.BString[j];
                myInstance.BString[j]=myInstance.BString[i];
                myInstance.BString[i]=temp;
            }
        }
    }
    place=0;

    //for each note in the key...
    for (int i = 0 ; i < 7 ; i++){
        //for each fret...
        for (int j = 0; j < 13 ; j++){
            //if the key and place on fretboard lines up
            if (chosenKey[i]==G[j]){
                myInstance.GString[place]=j;
                place++;
                GAMOUNT++;
            }
        }
    }
    //reorganize from smallest num to greatest (moving down the fretboard)
    for(int i = 0 ; i < GAMOUNT ; i++){
        for (int j = 0 ; j < GAMOUNT ; j++){
            if (myInstance.GString[i]<myInstance.GString[j]){
                temp = myInstance.GString[j];
                myInstance.GString[j]=myInstance.GString[i];
                myInstance.GString[i]=temp;
            }
        }
    }
    place=0;

    //for each note in the key...
    for (int i = 0 ; i < 7 ; i++){
        //for each fret...
        for (int j = 0; j < 13 ; j++){
            //if the key and place on fretboard lines up
            if (chosenKey[i]==D[j]){
                myInstance.DString[place]=j;
                place++;
                DAMOUNT++;
            }
        }
    }
    //reorganize from smallest num to greatest (moving down the fretboard)
    for(int i = 0 ; i < DAMOUNT ; i++){
        for (int j = 0 ; j < DAMOUNT ; j++){
            if (myInstance.DString[i]<myInstance.DString[j]){
                temp = myInstance.DString[j];
                myInstance.DString[j]=myInstance.DString[i];
                myInstance.DString[i]=temp;
            }
        }
    }
    place=0;

    //for each note in the key...
    for (int i = 0 ; i < 7 ; i++){
        //for each fret...
        for (int j = 0; j < 13 ; j++){
            //if the key and place on fretboard lines up
            if (chosenKey[i]==A[j]){
                myInstance.AString[place]=j;
                place++;
                AAMOUNT++;
            }
        }
    }
    //reorganize from smallest num to greatest (moving down the fretboard)
    for(int i = 0 ; i < AAMOUNT ; i++){
        for (int j = 0 ; j < AAMOUNT ; j++){
            if (myInstance.AString[i]<myInstance.AString[j]){
                temp = myInstance.AString[j];
                myInstance.AString[j]=myInstance.AString[i];
                myInstance.AString[i]=temp;
            }
        }
    }
    place=0;

    //for each note in the key...
    for (int i = 0 ; i < 7 ; i++){
        //for each fret...
        for (int j = 0; j < 13 ; j++){
            //if the key and place on fretboard lines up
            if (chosenKey[i]==E[j]){
                myInstance.EString[place]=j;
                place++;
                EAMOUNT++;
            }
        }
    }
    //reorganize from smallest num to greatest (moving down the fretboard)
    for(int i = 0 ; i < EAMOUNT ; i++){
        for (int j = 0 ; j < EAMOUNT ; j++){
            if (myInstance.EString[i]<myInstance.EString[j]){
                temp = myInstance.EString[j];
                myInstance.EString[j]=myInstance.EString[i];
                myInstance.EString[i]=temp;
            }
        }
    }
    place=0;
    
    /*for(int i = 0 ; i < highEAMOUNT ; i++){
        printf("%d ",myInstance.highEString[i]);
    }
    printf("\n");
    for(int i = 0 ; i < BAMOUNT ; i++){
        printf("%d ",myInstance.BString[i]);
    }
    printf("\n");
    for(int i = 0 ; i < GAMOUNT ; i++){
        printf("%d ",myInstance.GString[i]);
    }
    printf("\n");
    for(int i = 0 ; i < DAMOUNT ; i++){
        printf("%d ",myInstance.DString[i]);
    }
    printf("\n");
    for(int i = 0 ; i < AAMOUNT ; i++){
        printf("%d ",myInstance.AString[i]);
    }
    printf("\n");
    for(int i = 0 ; i < EAMOUNT ; i++){
        printf("%d ",myInstance.EString[i]);
    }*/
    printf("\n");
    //ASSIGN STUFF
    myDisp->myInstance=myInstance;
    myDisp->highEAMOUNT=highEAMOUNT;
    myDisp->BAMOUNT=BAMOUNT;
    myDisp->GAMOUNT=GAMOUNT;
    myDisp->DAMOUNT=DAMOUNT;
    myDisp->AAMOUNT=AAMOUNT;
    myDisp->EAMOUNT=EAMOUNT;
    //struct 2
    myDisp2->myInstance=myInstance;
    myDisp2->highEAMOUNT=highEAMOUNT;
    myDisp2->BAMOUNT=BAMOUNT;
    myDisp2->GAMOUNT=GAMOUNT;
    myDisp2->DAMOUNT=DAMOUNT;
    myDisp2->AAMOUNT=AAMOUNT;
    myDisp2->EAMOUNT=EAMOUNT;
    //struct 3
    myDisp3->myInstance=myInstance;
    myDisp3->highEAMOUNT=highEAMOUNT;
    myDisp3->BAMOUNT=BAMOUNT;
    myDisp3->GAMOUNT=GAMOUNT;
    myDisp3->DAMOUNT=DAMOUNT;
    myDisp3->AAMOUNT=AAMOUNT;
    myDisp3->EAMOUNT=EAMOUNT;
    int total;

    pthread_t thread, thread2, thread3;
    char tabLine[100];
    char LINES[25][100];
    char tempNum[3];
    int count=0;
    size_t n;

    
    if(strcmp(instChoice,"G")==0){
        myDisp->section=1;
        myDisp2->section=2;
        myDisp3->section=3;
        pthread_create(&thread, NULL, DisplayerGuitar, (void*)myDisp);
        pthread_create(&thread2, NULL, DisplayerGuitar, (void*)myDisp2);
        pthread_create(&thread3, NULL, DisplayerGuitar, (void*)myDisp3);
        pthread_join(thread, NULL);
        pthread_join(thread2, NULL);
        pthread_join(thread3, NULL);


    //calculate total number of dashes needed by seeing how many notes are in a section
    total = myDisp->highEcount+myDisp->Bcount+myDisp->Gcount+myDisp->Dcount+myDisp->Acount+myDisp->Ecount;
                                                                                                                    //READ ME!!!!!
                                                                                                                    /*
                                                                                                                    Most of the logic for the rest of this function is copy
                                                                                                                    pasted with only slight variations. For that reason
                                                                                                                    I will ONLY be logic-commenting out the first+second "block" 
                                                                                                                    because essentially the same thing happens
                                                                                                                    over and over for the guitar or bass section.
    //Subtract the highE amount to find out where to start placing numbers/notes                                                                                   */
    total-=myDisp->highEcount;
    //printf("|-");
        //clear buffer and print starting |-
    bzero(tabLine, 100);
    bzero(tempNum, 3);
    strcat(tabLine,"|-");
    //until numbers need to be placed, print dashes
    for (int i = 0; i*2 < total*4;i++){
        //printf("-");
        strcat(tabLine,"-");
    }
    //print out the needed numbers/notes
    for(int i = myDisp->highEStart ; i < myDisp->highEcount+myDisp->highEStart ; i++){
        //printf("%d-", myDisp->myInstance.highEString[i]);
        sprintf(tempNum, "%i",myDisp->myInstance.highEString[i]);
        strcat(tabLine, tempNum);
        strcat(tabLine, "-");
    }
    //cap off tab line with " | " and add to string array to be later printed. Clear variables and increment count
    strcat(tabLine,"|\n");
    //printf("|\n");
    strcpy(LINES[count], tabLine);
    count++;
    strcpy(tabLine,"");
    strcpy(tempNum,"");

        
    total-=myDisp->Bcount;
    //find out where to start placing numbers on the "B" line, clear buffers and start tab line for B
    bzero(tabLine, 100);
    bzero(tempNum, 3);
    strcat(tabLine,"|-");
    //print dashes until numbers start
    for (int i = 0; i*2 < total*4;i++){
       strcat(tabLine,"-");
    }
        //print note numbers
    for(int i = myDisp->BStart ; i < myDisp->Bcount+myDisp->BStart ; i++){
        sprintf(tempNum, "%i",myDisp->myInstance.BString[i]);
        strcat(tabLine, tempNum);
        strcat(tabLine, "-");
    }
    //print the difference for the amount of space missing to make the B tab line equal with the others
    for(int i = 0; i < myDisp->highEcount*2 ; i++){
        strcat(tabLine,"-");
    }
    //cap off tab line with " | " and add to string array to be later printed. Clear variables and increment count
    strcat(tabLine,"|\n");
    //printf("|\n");
    strcpy(LINES[count], tabLine);
    count++;
    strcpy(tabLine,"");
    strcpy(tempNum,"");
        
    

    total-=myDisp->Gcount;
    bzero(tabLine, 100);
    bzero(tempNum, 3);
    strcat(tabLine,"|-");
    for (int i = 0; i*2 < total*4;i++){
        strcat(tabLine,"-");
    }
    for(int i = myDisp->GStart ; i < myDisp->Gcount+myDisp->GStart ; i++){
        sprintf(tempNum, "%i",myDisp->myInstance.GString[i]);
        strcat(tabLine, tempNum);
        strcat(tabLine, "-");
    }

    for(int i = 0; i < myDisp->highEcount*2 + myDisp->Bcount*2; i++){
        strcat(tabLine,"-");
    }
    strcat(tabLine,"|\n");
    //printf("|\n");
    strcpy(LINES[count], tabLine);
    count++;
    strcpy(tabLine,"");
    strcpy(tempNum,"");


    

    total-=myDisp->Dcount;

    bzero(tabLine, 100);
    bzero(tempNum, 3);
    strcat(tabLine,"|-");
    for (int i = 0; i*2 < total*4;i++){
        strcat(tabLine,"-");
    }
    for(int i = myDisp->DStart ; i < myDisp->Dcount+myDisp->DStart ; i++){
        sprintf(tempNum, "%i",myDisp->myInstance.DString[i]);
        strcat(tabLine, tempNum);
        strcat(tabLine, "-");
    }

    for(int i = 0; i < myDisp->highEcount*2 + myDisp->Bcount*2 +myDisp->Gcount*2; i++){
        strcat(tabLine,"-");
    }
    strcat(tabLine,"|\n");
    //printf("|\n");
    strcpy(LINES[count], tabLine);
    count++;
    strcpy(tabLine,"");
    strcpy(tempNum,"");



    total-=myDisp->Acount;
    bzero(tabLine, 100);
    bzero(tempNum, 3);
    strcat(tabLine,"|-");
    for (int i = 0; i*2 < total*4;i++){
        strcat(tabLine,"-");
    }
    for(int i = myDisp->AStart ; i < myDisp->Acount+myDisp->AStart; i++){
       sprintf(tempNum, "%i",myDisp->myInstance.AString[i]);
        strcat(tabLine, tempNum);
        strcat(tabLine, "-");
    }

    for(int i = 0; i < myDisp->highEcount*2 +myDisp->Bcount*2 +myDisp->Gcount*2 + myDisp->Dcount*2; i++){
        strcat(tabLine,"-");
    }
    strcat(tabLine,"|\n");
    //printf("|\n");
    strcpy(LINES[count], tabLine);
    count++;
    strcpy(tabLine,"");
    strcpy(tempNum,"");

    total-=myDisp->Ecount;
    bzero(tabLine, 100);
    bzero(tempNum, 3);
    strcat(tabLine,"|-");
    for (int i = 0; i*2 < total*4;i++){
        strcat(tabLine,"-");
    }
    for(int i =myDisp-> EStart ; i < myDisp->Ecount+myDisp->EStart ; i++){
        sprintf(tempNum, "%i",myDisp->myInstance.EString[i]);
        strcat(tabLine, tempNum);
        strcat(tabLine, "-");
    }

    for(int i = 0; i < myDisp->highEcount*2 + myDisp->Bcount*2 +myDisp->Gcount*2 + myDisp->Dcount*2 + myDisp->Acount*2; i++){
        strcat(tabLine,"-");
    }
    strcat(tabLine,"|\n\n");
    //printf("|\n");
    strcpy(LINES[count], tabLine);
    count++;
    strcpy(tabLine,"");
    strcpy(tempNum,"");


        

   total = myDisp2->highEcount+myDisp2->Bcount+myDisp2->Gcount+myDisp2->Dcount+myDisp2->Acount+myDisp2->Ecount;
    total-=myDisp2->highEcount;
    //printf("|-");
    bzero(tabLine, 100);
    bzero(tempNum, 3);
    strcat(tabLine,"|-");
    for (int i = 0; i*2 < total*4;i++){
        //printf("-");
        strcat(tabLine,"-");
    }
    for(int i = myDisp2->highEStart ; i < myDisp2->highEcount+myDisp2->highEStart ; i++){
        //printf("%d-", myDisp->myInstance.highEString[i]);
        sprintf(tempNum, "%i",myDisp2->myInstance.highEString[i]);
        strcat(tabLine, tempNum);
        strcat(tabLine, "-");
    }
    strcat(tabLine,"|\n");
    //printf("|\n");
    strcpy(LINES[count], tabLine);
    count++;
    strcpy(tabLine,"");
    strcpy(tempNum,"");

        
    total-=myDisp2->Bcount;

    bzero(tabLine, 100);
    bzero(tempNum, 3);
    strcat(tabLine,"|-");
    for (int i = 0; i*2 < total*4;i++){
       strcat(tabLine,"-");
    }
    for(int i = myDisp2->BStart ; i < myDisp2->Bcount+myDisp2->BStart ; i++){
        sprintf(tempNum, "%i",myDisp2->myInstance.BString[i]);
        strcat(tabLine, tempNum);
        strcat(tabLine, "-");
    }

    for(int i = 0; i < myDisp2->highEcount*2 ; i++){
        strcat(tabLine,"-");
    }
    strcat(tabLine,"|\n");
    //printf("|\n");
    strcpy(LINES[count], tabLine);
    count++;
    strcpy(tabLine,"");
    strcpy(tempNum,"");
        
    

    total-=myDisp2->Gcount;
    bzero(tabLine, 100);
    bzero(tempNum, 3);
    strcat(tabLine,"|-");
    for (int i = 0; i*2 < total*4;i++){
        strcat(tabLine,"-");
    }
    for(int i = myDisp2->GStart ; i < myDisp2->Gcount+myDisp2->GStart ; i++){
        sprintf(tempNum, "%i",myDisp2->myInstance.GString[i]);
        strcat(tabLine, tempNum);
        strcat(tabLine, "-");
    }

    for(int i = 0; i < myDisp2->highEcount*2 + myDisp2->Bcount*2; i++){
        strcat(tabLine,"-");
    }
    strcat(tabLine,"|\n");
    //printf("|\n");
    strcpy(LINES[count], tabLine);
    count++;
    strcpy(tabLine,"");
    strcpy(tempNum,"");


    

    total-=myDisp2->Dcount;

    bzero(tabLine, 100);
    bzero(tempNum, 3);
    strcat(tabLine,"|-");
    for (int i = 0; i*2 < total*4;i++){
        strcat(tabLine,"-");
    }
    for(int i = myDisp2->DStart ; i < myDisp2->Dcount+myDisp2->DStart ; i++){
        sprintf(tempNum, "%i",myDisp2->myInstance.DString[i]);
        strcat(tabLine, tempNum);
        strcat(tabLine, "-");
    }

    for(int i = 0; i < myDisp2->highEcount*2 + myDisp2->Bcount*2 +myDisp2->Gcount*2; i++){
        strcat(tabLine,"-");
    }
    strcat(tabLine,"|\n");
    //printf("|\n");
    strcpy(LINES[count], tabLine);
    count++;
    strcpy(tabLine,"");
    strcpy(tempNum,"");



    total-=myDisp2->Acount;
    bzero(tabLine, 100);
    bzero(tempNum, 3);
    strcat(tabLine,"|-");
    for (int i = 0; i*2 < total*4;i++){
        strcat(tabLine,"-");
    }
    for(int i = myDisp2->AStart ; i < myDisp2->Acount+myDisp2->AStart; i++){
       sprintf(tempNum, "%i",myDisp2->myInstance.AString[i]);
        strcat(tabLine, tempNum);
        strcat(tabLine, "-");
    }

    for(int i = 0; i < myDisp2->highEcount*2 +myDisp2->Bcount*2 +myDisp2->Gcount*2 + myDisp2->Dcount*2; i++){
        strcat(tabLine,"-");
    }
    strcat(tabLine,"|\n");
    //printf("|\n");
    strcpy(LINES[count], tabLine);
    count++;
    strcpy(tabLine,"");
    strcpy(tempNum,"");

    total-=myDisp2->Ecount;
    bzero(tabLine, 100);
    bzero(tempNum, 3);
    strcat(tabLine,"|-");
    for (int i = 0; i*2 < total*4;i++){
        strcat(tabLine,"-");
    }
    for(int i =myDisp2-> EStart ; i < myDisp2->Ecount+myDisp2->EStart ; i++){
        sprintf(tempNum, "%i",myDisp2->myInstance.EString[i]);
        strcat(tabLine, tempNum);
        strcat(tabLine, "-");
    }

    for(int i = 0; i < myDisp2->highEcount*2 + myDisp2->Bcount*2 +myDisp2->Gcount*2 + myDisp2->Dcount*2 + myDisp2->Acount*2; i++){
        strcat(tabLine,"-");
    }
    strcat(tabLine,"|\n\n");
    //printf("|\n");
    strcpy(LINES[count], tabLine);
    count++;
    strcpy(tabLine,"");
    strcpy(tempNum,"");
    






    total = myDisp3->highEcount+myDisp3->Bcount+myDisp3->Gcount+myDisp3->Dcount+myDisp3->Acount+myDisp3->Ecount;
    total-=myDisp3->highEcount;
    bzero(tabLine, 100);
    bzero(tempNum, 3);
    strcat(tabLine,"|-");
    for (int i = 0; i*2 < total*4;i++){
        strcat(tabLine,"-");
    }
    for(int i = myDisp3->highEStart ; i < myDisp3->highEcount+myDisp3->highEStart ; i++){
       sprintf(tempNum, "%i",myDisp2->myInstance.highEString[i]);
        strcat(tabLine, tempNum);
        strcat(tabLine, "-");
    }
    if(myDisp->highEAMOUNT==7){
        strcat(tabLine,"-");
    }
    //SPECIAL CASES
    if (strcmp(myDisp->KEY,"C#")==0){
        strcat(tabLine,"-");
    } else if (strcmp(myDisp->KEY,"F")==0){
       strcat(tabLine,"-");
    } else if (strcmp(myDisp->KEY,"F#")==0){
        strcat(tabLine,"-");
    }  else if (strcmp(myDisp->KEY,"G#")==0){
        strcat(tabLine,"-");
    } 

    strcat(tabLine,"|\n");
    //printf("|\n");
    strcpy(LINES[count], tabLine);
    count++;
    strcpy(tabLine,"");
    strcpy(tempNum,"");

    total-=myDisp3->Bcount;

    bzero(tabLine, 100);
    bzero(tempNum, 3);
    strcat(tabLine,"|-");
    for (int i = 0; i*2 < total*4;i++){
        strcat(tabLine,"-");
    }
    for(int i =myDisp3-> BStart ; i < myDisp3->Bcount+myDisp3->BStart ; i++){
        sprintf(tempNum, "%i",myDisp2->myInstance.BString[i]);
        strcat(tabLine, tempNum);
        strcat(tabLine, "-");
    }

    for(int i = 0; i < myDisp3->highEcount*2 ; i++){
        strcat(tabLine,"-");
    }

    if(myDisp->BAMOUNT==7){
        strcat(tabLine,"-");
    }

    if (strcmp(myDisp->KEY,"C#")==0){
        strcat(tabLine,"-");
    } else if (strcmp(myDisp->KEY,"D#")==0){
        strcat(tabLine,"-");
    } else if (strcmp(myDisp->KEY,"F#")==0){
        strcat(tabLine,"-");
    }  else if (strcmp(myDisp->KEY, "G#")==0){
        strcat(tabLine,"-");
    } 

    
    strcat(tabLine,"|\n");
    //printf("|\n");
    strcpy(LINES[count], tabLine);
    count++;
    strcpy(tabLine,"");
    strcpy(tempNum,"");
    

    total-=myDisp3->Gcount;
    bzero(tabLine, 100);
    
    
    strcat(tabLine,"|-");
    for (int i = 0; i*2 < total*4;i++){
        strcat(tabLine,"-");
    }
    for(int i = myDisp3->GStart ; i < myDisp3->Gcount+myDisp3->GStart ; i++){
        sprintf(tempNum, "%i",myDisp2->myInstance.GString[i]);
        strcat(tabLine, tempNum);
        strcat(tabLine, "-");
    }

    for(int i = 0; i < myDisp3->highEcount*2 + myDisp3->Bcount*2; i++){
        strcat(tabLine,"-");
    }
    if(myDisp3->GAMOUNT==7){
        strcat(tabLine,"-");
    }

    if (strcmp(myDisp->KEY,"A#")==0){
        strcat(tabLine,"-");
    } else if (strcmp(myDisp->KEY,"D#")==0){
        strcat(tabLine,"-");
    } else if (strcmp(myDisp->KEY,"F")==0){
        strcat(tabLine,"-");
    }  else if (strcmp(myDisp->KEY,"G#")==0){
        strcat(tabLine,"-");
    } 
    strcat(tabLine,"|\n");
    //printf("|\n");
    strcpy(LINES[count], tabLine);
    count++;
    strcpy(tabLine,"");
    strcpy(tempNum,"");
    


    total-=myDisp3->Dcount;
    bzero(tabLine, 100);
    bzero(tempNum, 3);
    strcat(tabLine,"|-");
    for (int i = 0; i*2 < total*4;i++){
        strcat(tabLine,"-");
    }
    for(int i = myDisp3->DStart ; i < myDisp3->Dcount+myDisp3->DStart ; i++){
        sprintf(tempNum, "%i",myDisp2->myInstance.DString[i]);
        strcat(tabLine, tempNum);
        strcat(tabLine, "-");
    }

    for(int i = 0; i < myDisp3->highEcount*2 + myDisp3->Bcount*2 +myDisp3->Gcount*2; i++){
        strcat(tabLine,"-");
    }

    if(myDisp->DAMOUNT==7){
        strcat(tabLine,"-");
    }

    if (strcmp(myDisp->KEY,"A#")==0){
        strcat(tabLine,"-");
    } else if (strcmp(myDisp->KEY,"D#")==0){
        strcat(tabLine,"-");
    } else if (strcmp(myDisp->KEY,"F")==0){
        strcat(tabLine,"-");
    } else if (strcmp(myDisp->KEY,"F#")==0){
        strcat(tabLine,"-");
    } 

    strcat(tabLine,"|\n");
    //printf("|\n");
    strcpy(LINES[count], tabLine);
    count++;
    strcpy(tabLine,"");
    strcpy(tempNum,"");

    
    total-=myDisp3->Acount;
    bzero(tabLine, 100);
    bzero(tempNum, 3);
    strcat(tabLine,"|-");
    for (int i = 0; i*2 < total*4;i++){
        strcat(tabLine,"-");
    }
    for(int i = myDisp3->AStart ; i < myDisp3->Acount+myDisp3->AStart; i++){
        sprintf(tempNum, "%i",myDisp2->myInstance.AString[i]);
        strcat(tabLine, tempNum);
        strcat(tabLine, "-");
    }

    for(int i = 0; i < myDisp3->highEcount*2 + myDisp3->Bcount*2 +myDisp3->Gcount*2 + myDisp3->Dcount*2; i++){
        strcat(tabLine,"-");
    }
    if(myDisp->AAMOUNT==7){
        strcat(tabLine,"-");
    }

    if (strcmp(myDisp->KEY,"A#")==0){
        strcat(tabLine,"-");
    } else if (strcmp(myDisp->KEY,"C#")==0){
       strcat(tabLine,"-");
    } else if (strcmp(myDisp->KEY,"F")==0){
        strcat(tabLine,"-");
    } else if (strcmp(myDisp->KEY,"F#")==0){
        strcat(tabLine,"-");
    } 

    strcat(tabLine,"|\n");
    //printf("|\n");
    strcpy(LINES[count], tabLine);
    count++;
    strcpy(tabLine,"");
    strcpy(tempNum,"");

    total-=myDisp3->Ecount;
    bzero(tabLine, 100);
    bzero(tempNum, 3);
    strcat(tabLine,"|-");
    for (int i = 0; i*2 < total*4;i++){
        strcat(tabLine,"-");
    }
    for(int i = myDisp3->EStart ; i < myDisp3->Ecount+myDisp3->EStart ; i++){
        sprintf(tempNum, "%i",myDisp2->myInstance.EString[i]);
        strcat(tabLine, tempNum);
        strcat(tabLine, "-");
    }

    for(int i = 0; i < myDisp3->highEcount*2 + myDisp3->Bcount*2 +myDisp3->Gcount*2 + myDisp3->Dcount*2 + myDisp3->Acount*2; i++){
        strcat(tabLine,"-");
    }
    if(myDisp->EAMOUNT==7){
        strcat(tabLine,"-");
    }


    if (strcmp(myDisp->KEY,"C#")==0){
        strcat(tabLine,"-");
    } else if (strcmp(myDisp->KEY,"F")==0){
        strcat(tabLine,"-");
    } else if (strcmp(myDisp->KEY,"F#")==0){
        strcat(tabLine,"-");
    }  else if (strcmp(myDisp->KEY,"G#")==0){
       strcat(tabLine,"-");
    } 

    strcat(tabLine,"|\n\n");
    //printf("|\n");
    strcpy(LINES[count], tabLine);
    count++;
    strcpy(tabLine,"");
    strcpy(tempNum,"");
   bzero(tabLine, 100);
    bzero(tempNum, 3);
    

    } else if(strcmp(instChoice,"B")==0){
        myDisp->section=1;
        myDisp2->section=2;
        myDisp3->section=3;
        pthread_create(&thread, NULL, DisplayerGuitar, (void*)myDisp);
        pthread_create(&thread2, NULL, DisplayerGuitar, (void*)myDisp2);
        pthread_create(&thread3, NULL, DisplayerGuitar, (void*)myDisp3);
        pthread_join(thread, NULL);
        pthread_join(thread2, NULL);
        pthread_join(thread3, NULL);
         //printf("%i", highEcount);
    int total = myDisp->Gcount+myDisp->Dcount+myDisp->Acount+myDisp->Ecount;
    

    total-=myDisp->Gcount;
    bzero(tabLine, 100);
    bzero(tempNum, 3);
    strcat(tabLine,"|-");
    for (int i = 0; i*2 < total*4;i++){
        strcat(tabLine,"-");
    }
    for(int i = myDisp->GStart ; i < myDisp->Gcount+myDisp->GStart ; i++){
        sprintf(tempNum, "%i",myDisp2->myInstance.GString[i]);
        strcat(tabLine, tempNum);
        strcat(tabLine, "-");
    }


    strcat(tabLine,"|\n");
    //printf("|\n");
    strcpy(LINES[count], tabLine);
    count++;
    strcpy(tabLine,"");
    strcpy(tempNum,"");
    

    total-=myDisp->Dcount;

    bzero(tabLine, 100);
    bzero(tempNum, 3);
    strcat(tabLine,"|-");
    for (int i = 0; i*2 < total*4;i++){
        strcat(tabLine,"-");
    }
    for(int i = myDisp->DStart ; i < myDisp->Dcount+myDisp->DStart ; i++){
        sprintf(tempNum, "%i",myDisp2->myInstance.DString[i]);
        strcat(tabLine, tempNum);
        strcat(tabLine, "-");
    }

    for(int i = 0; i < myDisp->Gcount*2 ; i++){
       strcat(tabLine,"-");
    }
    strcat(tabLine,"|\n");
    //printf("|\n");
    strcpy(LINES[count], tabLine);
    count++;
    strcpy(tabLine,"");
    strcpy(tempNum,"");
    

    total-=myDisp->Acount;
    bzero(tabLine, 100);
    bzero(tempNum, 3);
    strcat(tabLine,"|-");
    for (int i = 0; i*2 < total*4;i++){
        strcat(tabLine,"-");
    }
    for(int i = myDisp->AStart ; i < myDisp->Acount+myDisp->AStart; i++){
        sprintf(tempNum, "%i",myDisp2->myInstance.AString[i]);
        strcat(tabLine, tempNum);
        strcat(tabLine, "-");
    }

    for(int i = 0; i < myDisp->Gcount*2 + myDisp->Dcount*2 ; i++){
        strcat(tabLine,"-");
    }
    strcat(tabLine,"|\n");
    //printf("|\n");
    strcpy(LINES[count], tabLine);
    count++;
    strcpy(tabLine,"");
    strcpy(tempNum,"");

    total-=myDisp->Ecount;
    bzero(tabLine, 100);
    bzero(tempNum, 3);
    strcat(tabLine,"|-");
    for (int i = 0; i*2 < total*4;i++){
       strcat(tabLine,"-");
    }
    for(int i = myDisp->EStart ; i < myDisp->Ecount+myDisp->EStart ; i++){
       sprintf(tempNum, "%i",myDisp2->myInstance.EString[i]);
        strcat(tabLine, tempNum);
        strcat(tabLine, "-");
    }

    for(int i = 0; i < myDisp->Gcount*2 + myDisp->Dcount*2 + myDisp->Acount*2; i++){
        strcat(tabLine,"-");
    }
    strcat(tabLine,"|\n\n");
    //printf("|\n");
    strcpy(LINES[count], tabLine);
    count++;
    strcpy(tabLine,"");
    strcpy(tempNum,"");
    

    //printf("%i", highEcount);
    total = myDisp2->Gcount+myDisp2->Dcount+myDisp2->Acount+myDisp2->Ecount;
    
    total-=myDisp2->Gcount;
    bzero(tabLine, 100);
    bzero(tempNum, 3);
    strcat(tabLine,"|-");
    for (int i = 0; i*2 < total*4;i++){
        strcat(tabLine,"-");
    }
    for(int i = myDisp2->GStart ; i <myDisp2-> Gcount+myDisp2->GStart ; i++){
        sprintf(tempNum, "%i",myDisp2->myInstance.GString[i]);
        strcat(tabLine, tempNum);
        strcat(tabLine, "-");
    }


    strcat(tabLine,"|\n");
    //printf("|\n");
    strcpy(LINES[count], tabLine);
    count++;
    strcpy(tabLine,"");
    strcpy(tempNum,"");
    

    total-=myDisp2->Dcount;

    bzero(tabLine, 100);
    bzero(tempNum, 3);
    strcat(tabLine,"|-");
    for (int i = 0; i*2 < total*4;i++){
        strcat(tabLine,"-");
    }
    for(int i = myDisp2->DStart ; i < myDisp2->Dcount+myDisp2->DStart ; i++){
        sprintf(tempNum, "%i",myDisp2->myInstance.DString[i]);
        strcat(tabLine, tempNum);
        strcat(tabLine, "-");
    }

    for(int i = 0; i < myDisp2->Gcount*2 ; i++){
        strcat(tabLine,"-");
    }
    strcat(tabLine,"|\n");
    //printf("|\n");
    strcpy(LINES[count], tabLine);
    count++;
    strcpy(tabLine,"");
    strcpy(tempNum,"");
    

    total-=myDisp2->Acount;
    bzero(tabLine, 100);
    bzero(tempNum, 3);
    strcat(tabLine,"|-");
    for (int i = 0; i*2 < total*4;i++){
        strcat(tabLine,"-");
    }
    for(int i = myDisp2->AStart ; i < myDisp2->Acount+myDisp2->AStart; i++){
        sprintf(tempNum, "%i",myDisp2->myInstance.AString[i]);
        strcat(tabLine, tempNum);
        strcat(tabLine, "-");
    }

    for(int i = 0; i < myDisp2->Gcount*2 + myDisp2->Dcount*2 ; i++){
        strcat(tabLine,"-");
    }
    strcat(tabLine,"|\n");
    //printf("|\n");
    strcpy(LINES[count], tabLine);
    count++;
    strcpy(tabLine,"");
    strcpy(tempNum,"");

    total-=myDisp2->Ecount;
    bzero(tabLine, 100);
    bzero(tempNum, 3);
    strcat(tabLine,"|-");
    for (int i = 0; i*2 < total*4;i++){
        strcat(tabLine,"-");
    }
    for(int i = myDisp2->EStart ; i < myDisp2->Ecount+myDisp2->EStart ; i++){
        sprintf(tempNum, "%i",myDisp2->myInstance.EString[i]);
        strcat(tabLine, tempNum);
        strcat(tabLine, "-");
    }

    for(int i = 0; i < myDisp2->Gcount*2 + myDisp2->Dcount*2 + myDisp2->Acount*2; i++){
       strcat(tabLine,"-");
    }
    strcat(tabLine,"|\n\n");
    //printf("|\n");
    strcpy(LINES[count], tabLine);
    count++;
    strcpy(tabLine,"");
    strcpy(tempNum,"");
    
    //printf("%i", Acount);
    total = myDisp3->Gcount+myDisp3->Dcount+myDisp3->Acount+myDisp3->Ecount;

    total-=myDisp3->Gcount;
    bzero(tabLine, 100);
    bzero(tempNum, 3);
    strcat(tabLine,"|-");
    for (int i = 0; i*2 < total*4;i++){
        strcat(tabLine,"-");
    }
    for(int i = myDisp3->GStart ; i < myDisp3->Gcount+myDisp3->GStart ; i++){
        sprintf(tempNum, "%i",myDisp2->myInstance.GString[i]);
        strcat(tabLine, tempNum);
        strcat(tabLine, "-");
    }
    if(myDisp->GAMOUNT==7){
        strcat(tabLine,"-");
    }

    if (strcmp(myDisp->KEY,"A#")==0){
        strcat(tabLine,"-");
    } else if (strcmp(myDisp->KEY,"D#")==0){
        strcat(tabLine,"-");
    } else if (strcmp(myDisp->KEY,"F")==0){
        strcat(tabLine,"-");
    }  else if (strcmp(myDisp->KEY,"G#")==0){
        strcat(tabLine,"-");
    } 
    strcat(tabLine,"|\n");
    //printf("|\n");
    strcpy(LINES[count], tabLine);
    count++;
    strcpy(tabLine,"");
    strcpy(tempNum,"");
    

    total-=myDisp3->Dcount;

    bzero(tabLine, 100);
    bzero(tempNum, 3);
    strcat(tabLine,"|-");
    for (int i = 0; i*2 < total*4;i++){
        strcat(tabLine,"-");
    }
    for(int i = myDisp3->DStart ; i < myDisp3->Dcount+myDisp3->DStart ; i++){
        sprintf(tempNum, "%i",myDisp2->myInstance.DString[i]);
        strcat(tabLine, tempNum);
        strcat(tabLine, "-");
    }

    for(int i = 0; i < myDisp3->Gcount*2 ; i++){
        strcat(tabLine,"-");
    }
    if(myDisp->DAMOUNT==7){
        strcat(tabLine,"-");
    }

    if (strcmp(myDisp->KEY,"A#")==0){
        strcat(tabLine,"-");
    } else if (strcmp(myDisp->KEY,"D#")==0){
        strcat(tabLine,"-");
    } else if (strcmp(myDisp->KEY,"F")==0){
       strcat(tabLine,"-");
    } else if (strcmp(myDisp->KEY,"F#")==0){
        strcat(tabLine,"-");
    } 
    strcat(tabLine,"|\n");
    //printf("|\n");
    strcpy(LINES[count], tabLine);
    count++;
    strcpy(tabLine,"");
    strcpy(tempNum,"");
    

    total-=myDisp3->Acount;
    bzero(tabLine, 100);
    bzero(tempNum, 3);
    strcat(tabLine,"|-");
    for (int i = 0; i*2 < total*4;i++){
        strcat(tabLine,"-");
    }
    for(int i = myDisp3->AStart ; i < myDisp3->Acount+myDisp3->AStart; i++){
        sprintf(tempNum, "%i",myDisp2->myInstance.AString[i]);
        strcat(tabLine, tempNum);
        strcat(tabLine, "-");
    }

    for(int i = 0; i < myDisp3->Gcount*2 + myDisp3->Dcount*2 ; i++){
        strcat(tabLine,"-");
    }
    if(myDisp->AAMOUNT==7){
        strcat(tabLine,"-");
    }

    if (strcmp(myDisp->KEY,"A#")==0){
        strcat(tabLine,"-");
    } else if (strcmp(myDisp->KEY,"C#")==0){
        strcat(tabLine,"-");
    } else if (strcmp(myDisp->KEY,"F")==0){
        strcat(tabLine,"-");
    } else if (strcmp(myDisp->KEY,"F#")==0){
        strcat(tabLine,"-");
    } 
    strcat(tabLine,"|\n");
    //printf("|\n");
    strcpy(LINES[count], tabLine);
    count++;
    strcpy(tabLine,"");
    strcpy(tempNum,"");

    total-=myDisp3->Ecount;
    bzero(tabLine, 100);
    bzero(tempNum, 3);
    strcat(tabLine,"|-");
    for (int i = 0; i*2 < total*4;i++){
        strcat(tabLine,"-");
    }
    for(int i = myDisp3->EStart ; i < myDisp3->Ecount+myDisp3->EStart ; i++){
        sprintf(tempNum, "%i",myDisp2->myInstance.EString[i]);
        strcat(tabLine, tempNum);
        strcat(tabLine, "-");
    }

    for(int i = 0; i < myDisp3->Gcount*2 + myDisp3->Dcount*2 + myDisp3->Acount*2; i++){
        strcat(tabLine,"-");
    }
    if(myDisp->EAMOUNT==7){
        strcat(tabLine,"-");
    }


    if (strcmp(myDisp->KEY,"C#")==0){
        strcat(tabLine,"-");
    } else if (strcmp(myDisp->KEY,"F")==0){
        strcat(tabLine,"-");
    } else if (strcmp(myDisp->KEY,"F#")==0){
        strcat(tabLine,"-");
    }  else if (strcmp(myDisp->KEY,"G#")==0){
        strcat(tabLine,"-");
    } 
    strcat(tabLine,"|\n\n");
    //printf("|\n");
    strcpy(LINES[count], tabLine);
    count++;
    strcpy(tabLine,"");
    strcpy(tempNum,"");
    }
        for (int i = 0 ; i < count ; i++){
            
            n = write(myDisp->connfd, LINES[i],strlen(LINES[i]));
            strcpy(LINES[i],"");
            
        }
}



//This funtion takes the legal notes based on client key selection and finds out how many per string there are PLUS what note they start at per section 
void *DisplayerGuitar(void *arg){
    DisplayThreadStuct *myDisp = (DisplayThreadStuct*)arg;
    //initialize;
    
    myDisp->highEcount=0;
    myDisp->Bcount=0;
    myDisp->Gcount=0;
    myDisp->Dcount=0;
    myDisp->Acount=0;
    myDisp->Ecount=0;

    if(myDisp->section==1){
        
        //We need to know how many dashes are needed before an actual numerical output is given for an indicidual string.
        //We will use a for loop to decide how many dashes there need to be

        //basically, if a legal note exists in between frets 0 adnd 4, save the starting note and increment a quanity counter
        for (int i = 0; i < 4 ; i++){
            if(myDisp->myInstance.highEString[i]<=4){
                if(myDisp->highEcount==0){
                    myDisp->highEStart=i;
                }
                myDisp->highEcount++;
                
            } 
            if (myDisp->myInstance.BString[i]<=4){
                if(myDisp->Bcount==0){
                    myDisp->BStart=i;
                }
                myDisp->Bcount++;
            } 
            if (myDisp->myInstance.GString[i]<=4){
                if(myDisp->Gcount==0){
                    myDisp->GStart=i;
                }
                myDisp->Gcount++;
            } 
            if (myDisp->myInstance.DString[i]<=4){
                if(myDisp->Dcount==0){
                    myDisp->DStart=i;
                }
                myDisp->Dcount++;
                //printf("%i ", DStart);
            } 
            if (myDisp->myInstance.AString[i]<=4){
                if(myDisp->Acount==0){
                    myDisp->AStart=i;
                }
                myDisp->Acount++;
            } 
            if (myDisp->myInstance.EString[i]<=4){
                if(myDisp->Ecount==0){
                    myDisp->EStart=i;
                }
                myDisp->Ecount++;
            }   
        }
   

    } else if (myDisp->section==2){

        //We need to know how many dashes are needed before an actual numerical output is given for an indicidual string.
        //We will use a for loop to decide how many dashes there need to be

        //basically, if a legal note exists in between frets 4 adnd 8, save the starting note and increment a quanity counter
        for (int i = 0; i < 8 ; i++){
            if(myDisp->myInstance.highEString[i]>=4 &&myDisp->myInstance.highEString[i]<=8){
                if(myDisp->highEcount==0){
                    myDisp->highEStart=i;
                }
                myDisp->highEcount++;
                
            } 
            if (myDisp->myInstance.BString[i]>=4 && myDisp->myInstance.BString[i]<=8){
                if(myDisp->Bcount==0){
                    myDisp->BStart=i;
                }
                myDisp->Bcount++;
            } 
            if (myDisp->myInstance.GString[i]>=4 && myDisp->myInstance.GString[i]<=8){
                if(myDisp->Gcount==0){
                    myDisp->GStart=i;
                }
                myDisp->Gcount++;
            } 
            if (myDisp->myInstance.DString[i]>=4 && myDisp->myInstance.DString[i]<=8){
                if(myDisp->Dcount==0){
                    myDisp->DStart=i;
                }
                myDisp->Dcount++;
                //printf("%i ", DStart);
            } 
            if (myDisp->myInstance.AString[i]>=4 && myDisp->myInstance.AString[i]<=8){
                if(myDisp->Acount==0){
                    myDisp->AStart=i;
                }
                myDisp->Acount++;
            } 
            if (myDisp->myInstance.EString[i]>=4 && myDisp->myInstance.EString[i]<=8){
                if(myDisp->Ecount==0){
                    myDisp->EStart=i;
                }
                myDisp->Ecount++;
            }   
        }
    //printf("%i", highEcount);
    

    } else if (myDisp->section==3){

        //We need to know how many dashes are needed before an actual numerical output is given for an indicidual string.
        //We will use a for loop to decide how many dashes there need to be

        //basically, if a legal note exists in between frets 8 and 12, save the starting note and increment a quanity counter
        for (int i = 0; i < 8 ; i++){
            if(myDisp->myInstance.highEString[i]>=8 && myDisp->myInstance.highEString[i]<=12){
                if(myDisp->highEcount==0){
                    myDisp->highEStart=i;
                }
                myDisp->highEcount++;
                
            } 
            if (myDisp->myInstance.BString[i]>=8 && myDisp->myInstance.BString[i]<=12){
                if(myDisp->Bcount==0){
                    myDisp->BStart=i;
                }
                myDisp->Bcount++;
            } 
            if (myDisp->myInstance.GString[i]>=8 && myDisp->myInstance.GString[i]<=12){
                if(myDisp->Gcount==0){
                    myDisp->GStart=i;
                }
                myDisp->Gcount++;
            } 
            if (myDisp->myInstance.DString[i]>=8 && myDisp->myInstance.DString[i]<=12){
                if(myDisp->Dcount==0){
                    myDisp->DStart=i;
                }
                myDisp->Dcount++;
                //printf("%i ", DStart);
            } 
            if (myDisp->myInstance.AString[i]>=8 && myDisp->myInstance.AString[i]<=12){
                if(myDisp->Acount==0){
                    myDisp->AStart=i;
                }
                myDisp->Acount++;
            } 
            if (myDisp->myInstance.EString[i]>=8 && myDisp->myInstance.EString[i]<=12){
                if(myDisp->Ecount==0){
                    myDisp->EStart=i;
                }
                myDisp->Ecount++;
            }   
        }

}
    return NULL;
}



void *DisplayerBass(void*arg){
    DisplayThreadStuct *myDisp = (DisplayThreadStuct*)arg;
    //initialize
    myDisp->highEcount=0;
    myDisp->Bcount=0;
    myDisp->Gcount=0;
    myDisp->Dcount=0;
    myDisp->Acount=0;
    myDisp->Ecount=0;

    if(myDisp->section==1){
            
        //We need to know how many dashes are needed before an actual numerical output is given for an indicidual string.
        //We will use a for loop to decide how many dashes there need to be
        for (int i = 0; i < 4 ; i++){
           
            if (myDisp->myInstance.GString[i]<=4){
                if(myDisp->Gcount==0){
                    myDisp->GStart=i;
                }
                myDisp->Gcount++;
            } 
            if (myDisp->myInstance.DString[i]<=4){
                if(myDisp->Dcount==0){
                    myDisp->DStart=i;
                }
                myDisp->Dcount++;
                //printf("%i ", DStart);
            } 
            if (myDisp->myInstance.AString[i]<=4){
                if(myDisp->Acount==0){
                    myDisp->AStart=i;
                }
                myDisp->Acount++;
            } 
            if (myDisp->myInstance.EString[i]<=4){
                if(myDisp->Ecount==0){
                    myDisp->EStart=i;
                }
                myDisp->Ecount++;
            }   
        }
   
           
    } else if (myDisp->section==2){

        //We need to know how many dashes are needed before an actual numerical output is given for an indicidual string.
        //We will use a for loop to decide how many dashes there need to be
        for (int i = 0; i < 8 ; i++){
           
            if (myDisp->myInstance.GString[i]>=4 && myDisp->myInstance.GString[i]<=8){
                if(myDisp->Gcount==0){
                    myDisp->GStart=i;
                }
                myDisp->Gcount++;
            } 
            if (myDisp->myInstance.DString[i]>=4 && myDisp->myInstance.DString[i]<=8){
                if(myDisp->Dcount==0){
                    myDisp->DStart=i;
                }
                myDisp->Dcount++;
                //printf("%i ", DStart);
            } 
            if (myDisp->myInstance.AString[i]>=4 && myDisp->myInstance.AString[i]<=8){
                if(myDisp->Acount==0){
                    myDisp->AStart=i;
                }
                myDisp->Acount++;
            } 
            if (myDisp->myInstance.EString[i]>=4 && myDisp->myInstance.EString[i]<=8){
                if(myDisp->Ecount==0){
                    myDisp->EStart=i;
                }
                myDisp->Ecount++;
            }   
        }
    
    } else if(myDisp->section==3){

        //We need to know how many dashes are needed before an actual numerical output is given for an indicidual string.
        //We will use a for loop to decide how many dashes there need to be
        for (int i = 0; i < 8 ; i++){

            if (myDisp->myInstance.GString[i]>=8 && myDisp->myInstance.GString[i]<=12){
                if(myDisp->Gcount==0){
                    myDisp->GStart=i;
                }
                myDisp->Gcount++;
            } 
            if (myDisp->myInstance.DString[i]>=8 && myDisp->myInstance.DString[i]<=12){
                if(myDisp->Dcount==0){
                    myDisp->DStart=i;
                }
                myDisp->Dcount++;
                //printf("%i ", DStart);
            } 
            if (myDisp->myInstance.AString[i]>=8 && myDisp->myInstance.AString[i]<=12){
                if(myDisp->Acount==0){
                    myDisp->AStart=i;
                }
                myDisp->Acount++;
            } 
            if (myDisp->myInstance.EString[i]>=8 && myDisp->myInstance.EString[i]<=12){
                if(myDisp->Ecount==0){
                    myDisp->EStart=i;
                }
                myDisp->Ecount++;
            }   
        } 
    }   
    return NULL;
}

int main(int argc, char *argv[]) {
  int listenfd;
  int *connfdp; // file descriptor to communicate with the client
  socklen_t clientlen;
  struct sockaddr_storage clientaddr; /* Enough space for any address */
  char client_hostname[MAXLINE], client_port[MAXLINE];
  pthread_t tid;


  if (argc != 2) {
    fprintf(stderr, "usage: %s <port>\n", argv[0]);
    exit(0);
  }

  listenfd = Open_listenfd(argv[1]);
  // Server runs in the infinite loop.
  // To stop the server process, it needs to be killed using the Ctrl+C key.
  while (1) {
    clientlen = sizeof(struct sockaddr_storage);
    // wait for the connection from the client.
    connfdp = Malloc(sizeof(int));
    *connfdp = Accept(listenfd, (SA *)&clientaddr, &clientlen);
    Getnameinfo((SA *)&clientaddr, clientlen, client_hostname, MAXLINE, client_port, MAXLINE, 0);


    // upon new connection, create a new thread
    Pthread_create(&tid, NULL, serverFunction, connfdp);
    printf("Connected to (%s, %s)\n", client_hostname, client_port);
  }

  exit(0);
}