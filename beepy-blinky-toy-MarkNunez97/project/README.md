#Lab 2: Blinky-Buzzy Toys
## Project Description 

In this assignment, I created a an "ocarina" from the Legend of Zelda games.
This "ocarina" uses 5 buttons, all of the top buttons, and the bottom left
button from the bottom board. all of the buttons play different notes, and
when pressed in the correct patter, it will play either:

song of storms
sarias song
zeldas lullaby

to play these songs, you need to press the following buttons:

zeldas lullaby:
       s1
       s3
       s4
       s1
       s3
       s4

sarias song:
       s2
       s4
       s1
       s2
       s4
       s1
       
song of storms:
       p1.3
       s2
       s3
       p1.3
       s2
       s3

## classes 

buttonMain.c

* Everything gets initialized here. (buzzer, watchdog, etc);

buzzer.c

* this class contains all the information about how to play songs or how to buzz
  a single note, it contains a completion song, sarias song, zeldas lullaby,
  etc.

led.c

* This class implements the logic of the red or green led lights, it flashes
  on and off depending on what you press, works for all buttons


Makefile

* contains "make clean" and "make load" to load up the program

notes.h

* this class contains all of the correct frequencies for playing every
  song. I was able to aquire the correct notes by using a tuner.

stateMachine.c

* contains a total of 16 states. you start in state 0 and  depending on what
  you press, you will either go to a state dedicated to a song, or a state
  dump, if you are in state dump you have to press the reset button.


switches.c

* this class does all of the logic for the pressing of the boards. it also
  contains a statemachine, and depending on what you press it will go to that
  state and play the note dedicated to that state.


switches.s

* assembly version of switches.c

wdInterruptHandler.c

* this class allows me to play all of the music in the buzzer.c. it the only
  class it updates is the state machine class function "state_update()"


