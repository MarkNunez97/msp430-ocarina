#include <msp430.h>
#include "switches.h"
#include "led.h"
#include "buzzer.h"
#include "notes.h"

char switch_state_down, switch_state_changed; /* effectively boolean */

char sw1_state_down, sw2_state_down, sw3_state_down, sw4_state_down;

char sw1_state_changed, sw2_state_changed, sw3_state_changed, sw4_state_changed;

static enum {state_A, state_left, state_right, state_up, state_down, state_empty} state = state_empty; 

short note;

static char 
switch_update_interrupt_sense()
{
  char p1val = P1IN;
  /* update switch interrupt to detect changes from current buttons */
  P1IES |= (p1val & BOTTOM_SWITCH);	/* if switch up, sense down */
  P1IES &= (p1val | ~BOTTOM_SWITCH);	/* if switch down, sense up */
  return p1val;
}

static char
switch_update_interrupt_sense2(){

  char p2val = P2IN;
  P2IES |= (p2val & TOP_SWITCHES);
  P2IES &= (p2val | ~TOP_SWITCHES);

  return p2val;
  
}

void 
switch_init()			/* setup switch */
{  
  P1REN |= BOTTOM_SWITCH;		/* enables resistors for switches */
  P1IE = BOTTOM_SWITCH;		/* enable interrupts from switches */
  P1OUT |= BOTTOM_SWITCH;		/* pull-ups for switches */
  P1DIR &= ~BOTTOM_SWITCH;		/* set switches' bits for input */
  switch_update_interrupt_sense();

  P2REN |= TOP_SWITCHES;
  P2IE = TOP_SWITCHES;
  P2OUT |= TOP_SWITCHES;
  P2DIR &= ~TOP_SWITCHES;
  switch_update_interrupt_sense2();
    
  led_update();
}

void
switch_interrupt_handler()
{
  char p1val = switch_update_interrupt_sense();
  switch_state_down = (p1val & SW0) ? 0 : 1; /* 0 when SW0 is up */
  if (switch_state_down){
    state = state_A;
    note = D5;
    switch_state_changed = 1;
  }

  char p2val = switch_update_interrupt_sense2();

  sw1_state_down = (p2val & SW1)? 0:1;
  if(sw1_state_down){
    state = state_left;
    note = B5;
    sw1_state_changed = 1;
      }
  
  sw2_state_down = (p2val & SW2)? 0:1;
  if(sw2_state_down){
    state = state_down;
    note = F5;
    sw2_state_changed = 1;
      }
  sw3_state_down = (p2val & SW3)? 0:1;
  if(sw3_state_down){
    state = state_up;
    note = D6;
    sw3_state_changed = 1;
  }
  
  sw4_state_down = (p2val & SW4)? 0:1;
  if (sw4_state_down){
    state = state_right;
    note = A5;
    sw4_state_changed = 1;
  }

  switch(state){

  case state_A:
    play_buzzer(switch_state_down, note);
    
    break;
    
  case state_left:
    play_buzzer(sw1_state_down, note);
    break;

  case state_down:
    play_buzzer(sw2_state_down, note);
    break;

  case state_up:
    play_buzzer(sw3_state_down, note);
    break;

  case state_right:
    play_buzzer(sw4_state_down, note);
    break;
  }
  
  switch_state_changed = 1;
  
  led_update();
}
