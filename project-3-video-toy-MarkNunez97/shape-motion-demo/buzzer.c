#include <msp430.h>
#include <stdlib.h>
#include "libTimer.h"
#include "buzzer.h"


/*
complete song
 */

static int rate_c[] = {1,1,3};
static short complete[] ={3000, 2000, 1000, 0};
static int duration_c = 0;
static int current_note_c=0;

static int d = 10;

static int note_length = 6;

static int counter = 0;

void buzzer_init()
{
    /* 
       Direct timer A output "TA0.1" to P2.6.  
        According to table 21 from data sheet:
          P2SEL2.6, P2SEL2.7, anmd P2SEL.7 must be zero
          P2SEL.6 must be 1
        Also: P2.6 direction must be output
    */
    timerAUpmode();		/* used to drive speaker */
    P2SEL2 &= ~(BIT6 | BIT7);
    P2SEL &= ~BIT7; 
    P2SEL |= BIT6;
    P2DIR = BIT6;               /* enable output to speaker (P2.6) */

    duration_c = rate_c[0];
    
                                /* start buzzing!!! */
}


void play_complete(){

  short note = complete[current_note_c];
  int length = sizeof(complete)/sizeof(short);
  
  if (current_note_c<length){
  buzzer_set_period(note);
  }  
  duration_c--;
 
 if (duration_c == 0) {
   current_note_c++;
   duration_c = rate_c[current_note_c];
  } 
 
}

void buzzer_set_period(short cycles)
{
  CCR0 = cycles; 
  CCR1 = cycles >> 1;		/* one half cycle */
}
