#include <msp430.h>
#include "led.h"
#include "switches.h"
#include "buzzer.h"

void led_init()
{
  P1DIR |= LEDS;		// bits attached to leds are output
  switch_state_changed = 1;
  sw1_state_changed = 1;
  led_update();
}

void led_update(){
  if (switch_state_changed) {
    char ledFlags = 0; /* by default, no LEDs on */

    ledFlags |= switch_state_down ? LED_GREEN : 0;
    ledFlags |= switch_state_down ? 0 : LED_RED;

    P1OUT &= (0xff - LEDS) | ledFlags; // clear bits for off leds
    P1OUT |= ledFlags;         // set bits for on leds


  }
  if (sw1_state_changed) {
    char ledFlags = 0; /* by default, no LEDs on */

    ledFlags |= sw1_state_down ? LED_GREEN : 0;
    ledFlags |= sw1_state_down ? 0 : LED_RED;

    P1OUT &= (0xff - LEDS) | ledFlags; // clear bits for off leds
    P1OUT |= ledFlags;         // set bits for on leds


  }

  if (sw2_state_changed) {
    char ledFlags = 0; /* by default, no LEDs on */

    ledFlags |= sw2_state_down ? LED_GREEN : 0;
    ledFlags |= sw2_state_down ? 0 : LED_RED;

    P1OUT &= (0xff - LEDS) | ledFlags; // clear bits for off leds
    P1OUT |= ledFlags;         // set bits for on leds


  }
  if (sw3_state_changed) {
    char ledFlags = 0; /* by default, no LEDs on */

    ledFlags |= sw3_state_down ? LED_GREEN : 0;
    ledFlags |= sw3_state_down ? 0 : LED_RED;

    P1OUT &= (0xff - LEDS) | ledFlags; // clear bits for off leds
    P1OUT |= ledFlags;         // set bits for on leds


  }
if (sw4_state_changed) {
    char ledFlags = 0; /* by default, no LEDs on */

    ledFlags |= sw4_state_down ? LED_GREEN : 0;
    ledFlags |= sw4_state_down ? 0 : LED_RED;

    P1OUT &= (0xff - LEDS) | ledFlags; // clear bits for off leds
    P1OUT |= ledFlags;         // set bits for on leds


  }


  
  switch_state_changed = 0;
  sw1_state_changed = 0;
  sw2_state_changed = 0;
  sw3_state_changed = 0;
  sw4_state_changed = 0;
}

