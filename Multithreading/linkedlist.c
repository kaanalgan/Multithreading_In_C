#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linkedlist.h"

//Initialize the root node -> allocate memory for data and the address of the next child.
SongNode *createPlayList()
{
    SongNode *newPlayList = (SongNode *)malloc(sizeof(SongNode));
    newPlayList->song = NULL;
    newPlayList->nextSong = NULL;
    return newPlayList;
}


SongNode *insertSong(Song *song, SongNode *playListHeader)
{
    if (song)
    { //Start the insert function only if the given song is not empty.
        SongNode *currentNode = playListHeader;
        if (currentNode->song == NULL)
        {
            currentNode->song = song;
            currentNode->nextSong = NULL;
            playListHeader = currentNode;
        }
        else
        {
            while (currentNode->nextSong != NULL)
            {
                currentNode = currentNode->nextSong;
            }
            currentNode->nextSong = (SongNode *)malloc(sizeof(SongNode));
            currentNode->nextSong->song = song;
            currentNode->nextSong->nextSong = NULL;
        }
    }
    else
    {
        printf("\nGiven playlist is not initiated.");
    }
    return playListHeader;
}


//Delete a song by a given name.
SongNode *deleteSong(char *name, SongNode *playListHeader)
{
    SongNode *current = playListHeader;
    SongNode *previous = NULL;

    //if list is empty
    if (playListHeader == NULL)
    {
        return NULL;
    }

    //navigate through list
    while (strcmp(current->song->name, name) != 0)
    {

        //if it is last node
        if (current->nextSong == NULL){
            return NULL;
        }

        else{
            previous = current;
            current = current->nextSong;
        }
    }

    if (current == playListHeader){
        playListHeader = playListHeader->nextSong;
    }

    else{
        previous->nextSong = current->nextSong;
    }
    
    return playListHeader;
}



int length(SongNode *playListHeader) {
   int length = 0;
   struct node *current;
	
   for(current = playListHeader; current != NULL; current = current->nextSong) {
      length++;
   }
	
   return length;
}



char *getRandomSongName(SongNode *playListHeader){
    int randomIndex = rand() % (length(playListHeader));
    SongNode *currentNode = playListHeader;
    char *songName;
    randomIndex--;

    while(randomIndex > 0){
        currentNode = currentNode->nextSong;
        randomIndex--;
    }

    songName = currentNode->song->name;
    return songName;
}



//Delete the first occurence of the song which has the given genre.
SongNode *deleteSongByGenre(char *genre, SongNode *playListHeader)
{
    SongNode *current = playListHeader;
    SongNode *previous = NULL;

    if (playListHeader == NULL)
    {
        return NULL;
    }

    while (strcmp(current->song->genre, genre) != 0){

        if (current->nextSong == NULL){
            return NULL;
        }

        else{
            previous = current;
            current = current->nextSong;
        }
    }

    if (current == playListHeader){
        playListHeader = playListHeader->nextSong;
    }

    else{
        previous->nextSong = current->nextSong;
    }
    return playListHeader;
}




//Returns 1 if a song with the given genre is present in the list, 0 if not.
int lookForGenre(char *genre, SongNode *playListHeader) {
    SongNode *currentNode = playListHeader;
    int genrePresent = 0;
    if(currentNode != NULL){
        while(strcmp(currentNode->song->genre, genre) != 0){
            if(currentNode->nextSong == NULL){
                return 0;
            }else{
                currentNode = currentNode->nextSong;
            }
        }
        genrePresent++;
    }
    return genrePresent;
}



//Returns 1 if a song which is younger than the given year is present in the list, 0 if not.
int lookForYounger(int year, SongNode *playListHeader) {
    SongNode *currentNode = (SongNode *) malloc(sizeof(SongNode));
    currentNode = playListHeader;
    int youngerPresent = 0;
    if(currentNode != NULL){
        while(currentNode->song->year > year){
            if(currentNode->nextSong == NULL){
                return 0;
            }else{
                currentNode = currentNode->nextSong;
            }
        }
        if(currentNode->song->year < year){youngerPresent++;}   
    }
    return youngerPresent;
}




//Delete the first occurence of the song which is older than the given year.
SongNode *deleteSongBefore(int year, SongNode *playListHeader) {
    SongNode *current = playListHeader;
    SongNode *previous = (SongNode *) malloc(sizeof(SongNode));

    if (playListHeader->song == NULL)
    {
        printf("\nGiven playList header is null!");
        return NULL;
    }

    while (current->song->year >= year){

        if (current->nextSong == NULL){
            return NULL;
        }

        else{
            previous = current;
            current = current->nextSong;
        }
    }

    if (current == playListHeader){
        playListHeader = playListHeader->nextSong;
    }

    else{
        previous->nextSong = current->nextSong;
    }
    return playListHeader;
}



void printCurrentList(SongNode *playListHeader)
{
    if (playListHeader != NULL)
    {
        SongNode *songNode = playListHeader;
        while (songNode != NULL)
        {
            printf("\n\n");
            printf("\n\t-> Song Name:  %s", songNode->song->name);
            printf("\n\t-> Song Genre: %s", songNode->song->genre);
            printf("\n\t-> Song Year:  %d", songNode->song->year);
            songNode = songNode->nextSong;
        }
    }
    else
    {
        printf("\n\t -> Given playlist is empty!");
    }
}