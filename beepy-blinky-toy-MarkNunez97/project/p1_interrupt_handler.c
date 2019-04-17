#include <msp430.h>
#include "switches.h"

/* Switch on P1 (S2) */
void
__interrupt_vec(PORT1_VECTOR) Port_1(){
  if (P1IFG & BOTTOM_SWITCH) {	      /* did a button cause this interrupt? */
    P1IFG &= ~BOTTOM_SWITCH;		      /* clear pending sw interrupts */
    switch_interrupt_handler();	/* single handler for all switches */
  }
}

void
__interrupt_vec(PORT2_VECTOR) Port_2(){
  if (P2IFG & TOP_SWITCHES){
    P2IFG &= ~TOP_SWITCHES;
    switch_interrupt_handler();
  }
  
}

