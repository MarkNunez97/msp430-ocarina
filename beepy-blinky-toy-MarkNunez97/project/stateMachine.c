#include <msp430.h>
#include "switches.h"
#include "led.h"
#include "buzzer.h"
#include "notes.h"

static enum {lullaby_one, lullaby_two, lullaby_three,
	     saria_one,   saria_two,   saria_three,
	     sos_one,     sos_two,     sos_three,
	     lullaby_complete, saria_complete, sos_complete,
	     state_start, state_dump} state = state_start;

static int count = 0;

void state_update(){

  switch(state){

    
  case state_start:

           
    if(switch_state_down){
     
      state = sos_one;
      count++;
     
    }else
    
    if(sw1_state_down){
      state = lullaby_one;
      count++;
    }else
    
    if(sw2_state_down){
      state = saria_one;
      count++;
    }else
    
    if(sw3_state_down){
      state = state_dump;
      count++;
    }else
    
    if(sw4_state_down){
      state = state_dump;
      count++;
    }
    
    break;
    //song 1
  case lullaby_one:
    
    
    if(sw3_state_down){
      state = lullaby_two;
      count++;
    }
    
    else if(switch_state_down ||sw1_state_down || sw2_state_down || sw4_state_down){
      state = state_dump;
      count++;
    }
    
    
    break;
  case lullaby_two:
    
    
    if (sw4_state_down){
      state = lullaby_three;
      count++;
    }
   
    else if (switch_state_down ||sw1_state_down || sw2_state_down || sw3_state_down) {
      state = state_dump;
      count++;
    }
    
    break;
  case lullaby_three:
        
    if (count == 6){
      state = lullaby_complete;
      break;
    }
    else if (sw1_state_down){
      state = lullaby_one;
      count++;
      
    }else if  (switch_state_down ||sw2_state_down || sw3_state_down || sw4_state_down){
      state = state_dump;
      count++;
	}
    
    break;
    // song 2
  case saria_one:

    if (sw4_state_down){
      state = saria_two;
      count++;
    }
      else if(switch_state_down ||sw1_state_down || sw2_state_down || sw3_state_down){
      state = state_dump;
      count++;
    }
   

    break;

  case saria_two:

    
    if (sw1_state_down){
      state = saria_three;
      count++;
    }

    else if(switch_state_down ||sw2_state_down || sw3_state_down || sw4_state_down){
      state = state_dump;
      count++;
    }
   

    break;

    
  case saria_three:

    if (count == 6){
      state = saria_complete;
      break;
    }

    else if (sw2_state_down){
      state = saria_one;
      count++;
    }  
    else if(switch_state_down ||sw1_state_down || sw3_state_down || sw4_state_down){
      state = state_dump;
      count++;
    }
   
    break;
    // song 3
  case sos_one:
    
    if (sw2_state_down){
      state = sos_two;
      count++;
    }
      
    else if(switch_state_down ||sw1_state_down || sw3_state_down || sw4_state_down){
      state = state_dump;
      count++;
    }

    break;

  case sos_two:

    if (sw3_state_down){
      state = sos_three;
      count++;
    }
      
    else if(switch_state_down ||sw1_state_down || sw2_state_down || sw4_state_down){
      state = state_dump;
      count++;
    }

    break;

  case sos_three:

    
    if (count == 6){
      state = sos_complete;
      break;
    }    
    else if (switch_state_down){
      state = sos_one;
      count++;
    }
      
    else if(sw1_state_down || sw2_state_down || sw3_state_down || sw4_state_down){
      state = state_dump;
      count++;
    }

    break;

  case lullaby_complete:
    play_complete();
    play_lullaby();
    // set_led(0,1);
   
    break;

  case saria_complete:

    play_complete();
    play_saria();

    break;

  case sos_complete:

    play_complete();
    play_storm();

    break;

  case state_dump:

    if (count == 6){
      state = state_start;
      count =0;
      break;
    }

    if (switch_state_down ||sw1_state_down || sw2_state_down || sw3_state_down || sw4_state_down ){
      state = state_dump;
      count++;
    }

    break;
  
  }
  
}
