#include <msp430.h>
#include <stdlib.h>
#include "libTimer.h"
#include "buzzer.h"
#include "notes.h"
static enum {state_lullaby, state_storm, state_saria, none} state = none;

/*
song lullaby
 */

static short lullaby[] ={B5, D6, A5, B5, D6, A5, B5, D6, A6, G6, D6,0};
static int rate_l[] = {9,4,12,9,4,12,9,4,9,4,12,0};
static int duration_l = 0;
static int current_note_l=0;

/*
song saria
 */

static short saria[] ={F5, A5, B5, F5, A5, B5, F5, A5, B5, E6, D6, B5, C6, B5, G5, E5, 0};
static int rate_s[] = {2,2,4,2,2,4,2,2,2,2,4,2,2,2,2,4,0};
static int duration_s = 0;
static int current_note_s=0;

/*
song sos
 */

static int rate_sos[] = {1,1,5,1,1,5,4,1,1,1,1,1,5,0};
static short storm[] ={D5, F5, D6, D5, F5, D6, E6, F6, E6, F6, E6, C6, A5, 0};
static int duration_sos = 0;
static int current_note_sos =0;

  /*
complete song
 */

static int rate_c[] = {1,1,1,1,2,4};
static short complete[] ={A5, B5, D6, E6, A6, 0};
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

    duration_l = rate_l[0];

    duration_s = rate_s[0];

    duration_sos = rate_sos[0];

    duration_c = rate_c[0];
    
                                /* start buzzing!!! */
}

void play_buzzer(char state ,short n){
  
  if (state == 1){
    buzzer_set_period(n);
  }else{
    buzzer_set_period(0);
  }
  
  
  
}

void play_lullaby(){

  
  short note = lullaby[current_note_l];
  int length = sizeof(lullaby)/sizeof(short);

  if (current_note_c>5){
  
  if (current_note_l<length){
  buzzer_set_period(note);
  }  
  duration_l--;
 
 if (duration_l == 0) {
   current_note_l++;
   duration_l = rate_l[current_note_l];
  }
 
  }
    
}

void play_saria(){

  short note = saria[current_note_s];
  int length = sizeof(saria)/sizeof(short);
  
  if (current_note_c>5){

  if (current_note_s<length){
  buzzer_set_period(note);
  }  

  duration_s--;
 
 if (duration_s == 0) {
   current_note_s++;
   duration_s = rate_s[current_note_s];
  }

  }
   
}

void play_storm(){

  short note = storm[current_note_sos];
  int length = sizeof(storm)/sizeof(short);

  if (current_note_c>5){

  if (current_note_sos<length){
  buzzer_set_period(note);
  }  
  duration_sos--;
 
 if (duration_sos == 0) {
   current_note_sos++;
   duration_sos = rate_sos[current_note_sos];
  } 
  } 
  
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
