#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "song.h"

//Create song object.
Song *createSong(char *name, char *genre, int year){
    Song *newSong = (Song *) malloc(sizeof(Song));
    strcpy((newSong->name), name);
    strcpy((newSong->genre), genre);
    newSong->year = year;
    return newSong;
}