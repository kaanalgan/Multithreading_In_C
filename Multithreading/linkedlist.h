#include "song.h"

/*Data type for storing songs*/

struct node {
    Song *song;
    struct node* nextSong;
};

typedef struct node SongNode;

SongNode * createPlayList(); //Initializes the root node of the linked-list(playList)

SongNode * insertSong(Song *song, SongNode *playListHeader); //Adds the given song to the end of the list.

SongNode * deleteSong(char* name, SongNode *playListHeader); //Deletes a song with the given name.

SongNode * deleteSongByGenre(char *genre, SongNode *playListHeader); //Deletes the first occurence of the song with the given genre.

SongNode * deleteSongBefore(int year, SongNode *playListHeader); //Deletes the first song that is older than the given year.

char *getRandomSongName(SongNode *playListHeader); //Returns a random song from the linkedlist.

int length(SongNode *playListHeader); //Returns the length of the list.

int lookForGenre(char *genre, SongNode *playListHeader); //Returns 1 if the given linkedlist contains any song having the given genre. 0 if otherwise.

int lookForYounger(int year, SongNode *playListHeader); //Returns 1 if the given linkedlist contains any song being older than the given year.

void printCurrentList(SongNode *playListHeader); // Prints the songs contained by the linked list one by one.