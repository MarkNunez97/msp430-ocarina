#include <msp430.h>
#include "buzzer.h"
#include "led.h"
#include "stateMachine.h"

void decisecond() 
{
  static char cnt = 0;		/* # deciseconds/frequecy change */
  if (++cnt > 2) {
    cnt = 0;
  }
}

void
__interrupt_vec(WDT_VECTOR) WDT(){	/* 250 interrupts/sec */
  static char second_count = 0, decisecond_count = 0;
  if (++second_count == 250) {
    second_count = 0;
  }
  if (++decisecond_count == 25) {
    state_update();
    decisecond_count = 0;
  }
  
}
