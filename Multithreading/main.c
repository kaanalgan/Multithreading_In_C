#include <stdio.h>
#include <stdlib.h>
#include "linkedlist.h"
#include "song.h"
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <string.h>
#include <time.h>
#define NUMBER_OF_THREADS 5

//defining global playlist variables.
SongNode *userPlayList;
SongNode *playList1;
SongNode *playList2;
Song *cutSong;


char genres[5][8]; // Used for random genre generation done in thread B.


/*Global signal variables -> 0 indicates that particular thread is not done, 1 indicates otherwise. */
int firstAThreadCompleted = 0; 
int secondAThreadCompleted = 0;
int thirdAThreadCompleted = 0;
int bThreadIsCompleted = 0;
int cThreadIsCompleted = 0;
int extraDeletionIsDone = 0; //Signal variable -> 0 means playList1 & playList2 might not be empty , 1 means they're %100 empty.

int randomYear;
char *randomGenre;

/* Defining mutexes for each resource */
pthread_mutex_t userPlayList_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t playList1_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t playList2_lock = PTHREAD_MUTEX_INITIALIZER;




void threadA_task(int id){
    printf("\n TYPE A THREAD WITH ID(%d) HAS STARTED ITS EXECUTION.\n", id);
    /*Type A threads initially lock userplaylist & playlist2 to perfrom its cut/paste operations*/
    pthread_mutex_trylock(&userPlayList_lock); 
    pthread_mutex_trylock(&playList2_lock);

    /*Generating the number of cut/paste operations -> [1,10]*/
    int loopCounter1 = 1 + rand() / (RAND_MAX / (10 - 1 + 1) + 1);
    printf("\n\t -> # of cut(s)/paste(s) generated inside thread (%d): %d\n", id, loopCounter1);

    /*Allocate memory for a temporary song which will be used in cut/paste operations */
    cutSong = (Song *) malloc(sizeof(Song));


    /*Cut from playlist2 and then insert into userPlaylist*/
    while(playList2 != NULL && loopCounter1 > 0){
        cutSong = playList2->song;
        playList2 = deleteSong(cutSong->name, playList2);
        userPlayList = insertSong(cutSong, userPlayList);
        loopCounter1--;
    }
    pthread_mutex_unlock(&playList2_lock); //Release playlist2 when done.

    pthread_mutex_trylock(&playList1_lock);
    /*Generating the number of cut/paste operations -> [1,10]*/
    int loopCounter2 = 1 + rand() / (RAND_MAX / (10 - 1 + 1) + 1);    
    printf("\n\t -> # of cut(s)/paste(s) generated inside thread (%d): %d\n", id, loopCounter2);

    /*Cut from playList1 and then insert into userPlayList*/
    while(playList1 != NULL && loopCounter2 > 0){
        cutSong = playList1->song;
        playList1 = deleteSong(cutSong->name, playList1);
        userPlayList = insertSong(cutSong, userPlayList);
        loopCounter2--;
    }
    pthread_mutex_unlock(&playList1_lock);


    /*Before releasing the userplaylist signal other threads that the cut/paste operations are done.*/
    switch(id){
        case 0: //Thread A with id=0
            firstAThreadCompleted++;
            printf("\n******First A thread with id(%d) is done.******\n", id);
            break;

        case 1: //Thread A with id=1
            secondAThreadCompleted++;
            printf("\n******Second A thread with id(%d) is done.******\n", id);
            break;

        case 2: //Thread A with id=2
            thirdAThreadCompleted++;
            printf("\n******Third A thread with id (%d) is done.******\n", id);
            break;

        default:
            printf("\n\tUNEXPECTED THREAD EVENT.");
            break;

    }
    pthread_mutex_unlock(&userPlayList_lock);
    sleep(0.8); /*Wait for other A threads to claim userplaylist to prevent any possible starvations.*/
    
    
    pthread_mutex_trylock(&userPlayList_lock);
    /*While playlist1/playlist2 is not empty -> proceed cut/paste process.*/
    while(playList1 != NULL){
        cutSong = playList1->song;
        playList1 = deleteSong(cutSong->name, playList1);
        userPlayList = insertSong(cutSong, userPlayList);
    }
    while(playList2 != NULL){
        cutSong = playList2->song;
        playList2 = deleteSong(cutSong->name, playList2);
        userPlayList = insertSong(cutSong, userPlayList);
    }
    extraDeletionIsDone++;
    pthread_mutex_unlock(&userPlayList_lock);
    pthread_exit(NULL);
}




void threadB_task(int id){
    printf("\n TYPE B THREAD WITH ID(%d) HAS STARTED ITS EXECUTION.\n", id);

    /*Lock playlist2*/
    pthread_mutex_trylock(&playList2_lock);
    /*Look for a song in playList2 with a particular genre while such a song with the genre exists -> delete it*/
    while(lookForGenre(randomGenre, playList2)){playList2 = deleteSongByGenre(randomGenre, playList2);}
    pthread_mutex_unlock(&playList2_lock);

    /*Lock playlist1*/
    pthread_mutex_trylock(&playList1_lock);
    /*Look for a song in playList1 with a particular genre while such a song with the genre exists -> delete it */
    while(lookForGenre(randomGenre, playList1)){playList1 = deleteSongByGenre(randomGenre, playList1);}
    pthread_mutex_unlock(&playList1_lock);
    
    //wait for all A threads to complete its work to reach userplaylist.
    while(firstAThreadCompleted != 1 && secondAThreadCompleted != 1 && thirdAThreadCompleted != 1); 
    pthread_mutex_trylock(&userPlayList_lock);
    while(lookForGenre(randomGenre, userPlayList)){userPlayList = deleteSongByGenre(randomGenre, userPlayList);}
    pthread_mutex_unlock(&userPlayList_lock);
    printf("\n******THREAD TYPE B WITH ID(%d) ACCOMPLISHED ITS TASK!********\n", id);
    pthread_exit(NULL);
}



void threadC_task(int id){
    printf("\n TYPE C THREAD WITH ID(%d) HAS STARTED ITS EXECUTION.\n", id);

    /*Lock playlist2 first*/
    pthread_mutex_trylock(&playList2_lock);
    while(lookForYounger(randomYear, playList2)){playList2 = deleteSongBefore(randomYear, playList2);}
    pthread_mutex_unlock(&playList2_lock);
    
    /*Lock playlist1 secondly*/
    pthread_mutex_trylock(&playList1_lock);
    while(lookForYounger(randomYear, playList1)){playList1 = deleteSongBefore(randomYear, playList1);}
    pthread_mutex_unlock(&playList1_lock);


    //wait for all A threads to complete its work to reach userplaylist.
    while(firstAThreadCompleted != 1 && secondAThreadCompleted != 1 && thirdAThreadCompleted != 1 && extraDeletionIsDone != 1); 
    pthread_mutex_trylock(&userPlayList_lock);
    while(lookForYounger(randomYear, userPlayList)){userPlayList = deleteSongBefore(randomYear, userPlayList);}
    pthread_mutex_unlock(&userPlayList_lock);
    printf("\n******THREAD TYPE C WITH ID(%d) ACCOMPLISHED ITS TASK!******\n", id);
    pthread_exit(NULL);
}


//Assembles all functions -> All threads initially gets inside this function and according to their ids they get categorized and call the corresponding function.
void *assignThreadsToFunctions(void *id){
    int *idAsInt = (int *) id;
     if(*idAsInt == 0 || *idAsInt == 1 || *idAsInt == 2)   {threadA_task(*idAsInt);}     // Thread A for id=0, 1, 2
     else if(*idAsInt == 3)  {threadB_task(*idAsInt);}                                   // Thread B for id=3
     else   {threadC_task(*idAsInt);}                                                    // Thread C for id=4
}



int main(){

    srand(time(NULL));
    
    /*Generate a random year between 1980 and 1994 */
    randomYear = 1980 + rand() / (RAND_MAX / (1994 - 1980 + 1) + 1);
    printf("\n********************************"); printf("\t************************************");
    printf("\nRandomly generated year is: %d", randomYear);

    /*List containing all possible genres for random genre generation.*/
    strcpy(genres[0], "Genre#1");   
    strcpy(genres[1], "Genre#2");             
    strcpy(genres[2], "Genre#3");
    strcpy(genres[3], "Genre#4");
    strcpy(genres[4], "Genre#5");

    /*Generate a random genre */
    randomGenre = (char *) malloc(8 * sizeof(char));
    randomGenre = genres[rand() % 5];
    printf("\tRandomly generated genre is: %s", randomGenre);


    printf("\n********************************"); printf("\t************************************");

    printf("\n");
    printf("\n");

    //Initializing the playlists.
    userPlayList = createPlayList();
    playList1 = createPlayList();
    playList2 = createPlayList();


    //Inserting songs into playlist1.
    playList1 = insertSong(createSong("Track#1", "Genre#1", 1980), playList1);
    playList1 = insertSong(createSong("Track#2", "Genre#2", 1981), playList1);
    playList1 = insertSong(createSong("Track#3", "Genre#3", 1982), playList1);
    playList1 = insertSong(createSong("Track#4", "Genre#4", 1983), playList1);
    playList1 = insertSong(createSong("Track#5", "Genre#5", 1984), playList1);
    playList1 = insertSong(createSong("Track#6", "Genre#1", 1985), playList1);
    playList1 = insertSong(createSong("Track#7", "Genre#2", 1986), playList1);
    playList1 = insertSong(createSong("Track#8", "Genre#3", 1987), playList1);
    playList1 = insertSong(createSong("Track#9", "Genre#4", 1988), playList1);
    playList1 = insertSong(createSong("Track#10","Genre#5", 1989), playList1);
    playList1 = insertSong(createSong("Track#11", "Genre#1", 1990), playList1);
    playList1 = insertSong(createSong("Track#12", "Genre#2", 1991), playList1);
    playList1 = insertSong(createSong("Track#13", "Genre#3", 1992), playList1);
    playList1 = insertSong(createSong("Track#14", "Genre#4", 1993), playList1);
    playList1 = insertSong(createSong("Track#15", "Genre#5", 1994), playList1);

    //Inserting songs into playlist2.
    playList2 = insertSong(createSong("Track#16", "Genre#1", 1980), playList2);
    playList2 = insertSong(createSong("Track#17", "Genre#2", 1981), playList2);
    playList2 = insertSong(createSong("Track#18", "Genre#3", 1982), playList2);
    playList2 = insertSong(createSong("Track#19", "Genre#4", 1983), playList2);
    playList2 = insertSong(createSong("Track#20", "Genre#5", 1984), playList2);
    playList2 = insertSong(createSong("Track#21", "Genre#1", 1985), playList2);
    playList2 = insertSong(createSong("Track#22", "Genre#2", 1986), playList2);
    playList2 = insertSong(createSong("Track#23", "Genre#3", 1987), playList2);
    playList2 = insertSong(createSong("Track#24", "Genre#4", 1988), playList2);
    playList2 = insertSong(createSong("Track#25", "Genre#5", 1989), playList2);
    playList2 = insertSong(createSong("Track#26", "Genre#1", 1990), playList2);
    playList2 = insertSong(createSong("Track#27", "Genre#2", 1991), playList2);
    playList2 = insertSong(createSong("Track#28", "Genre#3", 1992), playList2);
    playList2 = insertSong(createSong("Track#29", "Genre#4", 1993), playList2);
    playList2 = insertSong(createSong("Track#30", "Genre#5", 1994), playList2);


    //Initializing threads & attributes.
    pthread_t tid[5];
    pthread_attr_t attr[5];

    //Creating threads
    for(int i=0; i<NUMBER_OF_THREADS; i++){
        void *memory = malloc(sizeof(int));
        int* id = (int *) memory;
        *id = i;
        pthread_attr_init(&attr[i]);
        pthread_create(&tid[i], &attr[i], assignThreadsToFunctions, id);
    }


    //Wait for all the threads to finish its work.
    for(int i=0; i<NUMBER_OF_THREADS; i++){
        pthread_join(tid[i], NULL);
    }


    //Print lists after threads are done.
    printf("\n\n*********************************");
    printf("\nFinal lists are given below: ");
    printf("\n*********************************");
    printf("\n\n\tUSER PLAYLIST");
    printf("\n\t-------------");
    printCurrentList(userPlayList);
    printf("\n\n\n\tPLAYLIST#1");
    printf("\n\t----------");
    printCurrentList(playList1);
    printf("\n\n\n\tPLAYLIST#2");
    printf("\n\t----------");
    printCurrentList(playList2);
    printf("\n");

    return 0;
}