## Play some classic games in a terminal!  
#### requirements:  
C compiler installed  

compile the game you want to play with:  
```
gcc connect-4.c -o connect-4
gcc ballsort.c -o ballsort
gcc hangman.c -o hangman
```
(You can use any compiler you want)

#### ! for compiling crossy-road.c you have to have curses library installed !
```
gcc crossy-road.c -lcurses -o crossy-road
```

Then run your game with:
```
./<executable_game_name>
```
