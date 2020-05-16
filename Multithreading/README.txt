------------------------------------------------------
MORE IMPLEMENTATION DETAILS AND HOW THE PROGRAM WORKS:
------------------------------------------------------
    # Inside threads B & C instead of getting input from the user ->  Program generates a random genre & a random year between 1980(min) - 1994(max)
    # Thread A after performing the random number of cut/pastes, sleeps for a while to give a chance for other type A threads to claim the resources(preventing starvation) and then if either of the type1 lists are not
    empty proceed the cut/paste until both are empty. 
    
    
    main.c -> Creates playlist1, playlist2, and userplaylist. Creates 5 threads, categorizes them and then according to their classes(A, B, C) they invoke the corresponding method. 
    linkedlist.c -> data structure used for holding song nodes -> a node contains: Song *song, SongNode *nextSong
    song.c -> basic data structure for storing a particular song's name, genre and year.

----------
EXECUTION:
----------
    gcc main.c song.c linkedlist.c -o mainout -lpthread
    ./mainout