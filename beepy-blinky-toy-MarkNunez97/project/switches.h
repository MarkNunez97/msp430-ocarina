#ifndef switches_included
#define switches_included

#define SW0 BIT3                /* switch1 is p1.3 */

#define SW1 BIT0               /* SW1 is p2.0 */
#define SW2 BIT1               /* SW2 is p2.1 */
#define SW3 BIT2               /* SW3 is p2.2 */
#define SW4 BIT3               /* SW4 is p2.3 */



#define BOTTOM_SWITCH SW0		/* only 1 switch on this board */
#define TOP_SWITCHES (SW1 | SW2 | SW3 | SW4)

void switch_init();
void switch_interrupt_handler();
void play_buzzer(char state, short note);
extern char switch_state_down, switch_state_changed; /* effectively boolean */

extern char sw1_state_down, sw2_state_down, sw3_state_down, sw4_state_down;

extern char sw1_state_changed,sw2_state_changed,sw3_state_changed,sw4_state_changed;
#endif // included
