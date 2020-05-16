#ifndef song_h
#define song_h

struct song {
    char name [25];
    char genre [15];
    int year;
};

typedef struct song Song;

Song * createSong(char *name, char *genre, int year);

#endif