#Lab 3: LCD Game
## Project Description 

In this assignment, I created a game where you must avoid your enemies. if you
fail to do so, you will die.

sw0 - move left
sw1 = move up
sw2 - move down
sw3 - move right

## classes 

AbEnemy.c

* in this class i create a custom shape you will have to avoid

abPlayer.c

* in this class i create a custom shape that you can control

buzzer.c

* this class projects a noise when the enemies collide with the wall

shapemotion.c

* this class is what initializes everything and has the logic and shapes that
  are contained in the game.

Makefile

* contains "make clean" and "make load" to load up the program
