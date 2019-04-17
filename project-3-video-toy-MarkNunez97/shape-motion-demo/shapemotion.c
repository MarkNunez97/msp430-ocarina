/** \file shapemotion.c
 *  \brief This is a simple shape motion demo.
 *  This demo creates two layers containing shapes.
 *  One layer contains a rectangle and the other a circle.
 *  While the CPU is running the green LED is on, and
 *  when the screen does not need to be redrawn the CPU
 *  is turned off along with the green LED.
 */  
#include <msp430.h>
#include <libTimer.h>
#include <lcdutils.h>
#include <lcddraw.h>
#include <p2switches.h>
#include "shape.h"
#include <abCircle.h>
#include "buzzer.h"
#include <stdlib.h>
#define GREEN_LED BIT6

u_char intersection(Vec2 *p, Region* r);

// parameters for all shapes/////////////////////////////////////////////////////

AbRectOutline fieldOutline = {	/* playing field */
  abRectOutlineGetBounds, abRectOutlineCheck,   
  {screenWidth/2-1, screenHeight/2-20}
};

AbRect rect14 = {abRectGetBounds, abRectCheck, {1,1}};; /**< 10x10 rectangle */

AbEnemy enemy1 = {abEnemyGetBounds, abEnemyCheck, 10};

/////////////////////////////////////////////////////////////////////////////////

// player object///////////////////////////////////////////////////////////////////

AbPlayer player1 = {abPlayerGetBounds, abPlayerCheck, 10};

Layer player = {		/**< Layer with an orange circle */
  (AbShape *)&player1,
  {(screenWidth/2), (screenHeight/2)}, /**< bit below & right of center */
  {0,0}, {0,0},				    /* last & next pos */
  COLOR_BLACK,
  0,
};

/////////////////////////////////////////////////////////////////////////////////

  

Layer layer4 = {		/**< Layer with an orange circle */
  (AbShape *)&enemy1,
  {(screenWidth/2)-20, (screenHeight/2)+20}, /**< bit below & right of center */
  {0,0}, {0,0},				    /* last & next pos */
  COLOR_WHITE,
  &player,
};
  
Layer layer3 = {		/**< Layer with an orange circle */
  (AbShape *)&enemy1,
  {(screenWidth/2)+20, (screenHeight/2)-20}, /**< bit below & right of center */
  {0,0}, {0,0},				    /* last & next pos */
  COLOR_WHITE,
  &layer4,
};


Layer fieldLayer = {		/* playing field as a layer */
  (AbShape *) &fieldOutline,
  {screenWidth/2, screenHeight/2},/**< center */
  {0,0}, {0,0},				    /* last & next pos */
  COLOR_GREEN,
  &layer3
};

Layer layer1 = {		/**< Layer with a red square */
  (AbShape *)&enemy1,
  {(screenWidth/2)-20, (screenHeight/2)-20}, /**< center */
  {0,0}, {0,0},				    /* last & next pos */
  COLOR_WHITE,
  &fieldLayer,
};

Layer layer0 = {		/**< Layer with an orange circle */
  (AbShape *)&enemy1,
  {(screenWidth/2)+20, (screenHeight/2)+20}, /**< bit below & right of center */
  {0,0}, {0,0},				    /* last & next pos */
  COLOR_WHITE,
  &layer1,
};

/** Moving Layer
 *  Linked list of layer references
 *  Velocity represents one iteration of change (direction & magnitude)
 */
typedef struct MovLayer_s {
  Layer *layer;
  Vec2 velocity;
  struct MovLayer_s *next;
} MovLayer;

/* initial value of {0,0} will be overwritten */
MovLayer movPlayer = { &player, {0,0}, 0 }; /**< not all layers move */


MovLayer ml4 = { &layer4, {-3,2}, 0 }; /**< not all layers move */
MovLayer ml3 = { &layer3, {2,-2}, &ml4 }; /**< not all layers move */
MovLayer ml1 = { &layer1, {-2,-2}, &ml3 }; 
MovLayer ml0 = { &layer0, {2,2}, &ml1 }; 

void movLayerDraw(MovLayer *movLayers, Layer *layers)
{
  int row, col;
  MovLayer *movLayer;

  and_sr(~8);			/**< disable interrupts (GIE off) */
  for (movLayer = movLayers; movLayer; movLayer = movLayer->next) { /* for each moving layer */
    Layer *l = movLayer->layer;
    l->posLast = l->pos;
    l->pos = l->posNext;
  }
  or_sr(8);			/**< disable interrupts (GIE on) */


  for (movLayer = movLayers; movLayer; movLayer = movLayer->next) { /* for each moving layer */
    Region bounds;
    layerGetBounds(movLayer->layer, &bounds);
    lcd_setArea(bounds.topLeft.axes[0], bounds.topLeft.axes[1], 
		bounds.botRight.axes[0], bounds.botRight.axes[1]);
    for (row = bounds.topLeft.axes[1]; row <= bounds.botRight.axes[1]; row++) {
      for (col = bounds.topLeft.axes[0]; col <= bounds.botRight.axes[0]; col++) {
	Vec2 pixelPos = {col, row};
	u_int color = bgColor;
	Layer *probeLayer;
	for (probeLayer = layers; probeLayer; 
	     probeLayer = probeLayer->next) { /* probe all layers, in order */
	  if (abShapeCheck(probeLayer->abShape, &probeLayer->pos, &pixelPos)) {
	    color = probeLayer->color;
	    break; 
	  } /* if probe check */
	} // for checking all layers at col, row
	lcd_writeColor(color); 
      } // for col
    } // for row
  } // for moving layer being updated
}	  



//Region fence = {{10,30}, {SHORT_EDGE_PIXELS-10, LONG_EDGE_PIXELS-10}}; /**< Create a fence region */

/** Advances a moving shape within a fence
 *  
 *  \param ml The moving shape to be advanced
 *  \param fence The region which will serve as a boundary for ml
 */
void mlAdvance(MovLayer *ml, Region *fence)
{
  Vec2 newPos;
  
  u_char axis;
  Region shapeBoundary;
  
  
  for (; ml; ml = ml->next) {
    vec2Add(&newPos, &ml->layer->posNext, &ml->velocity);

    abShapeGetBounds(ml->layer->abShape, &newPos, &shapeBoundary);
    
    for (axis = 0; axis < 2; axis ++) {
      if ((shapeBoundary.topLeft.axes[axis] < fence->topLeft.axes[axis]) ||
	  (shapeBoundary.botRight.axes[axis] > fence->botRight.axes[axis]) ) {
	int velocity = ml->velocity.axes[axis] = -ml->velocity.axes[axis];

	int timer = 0;
	
	while(timer <1500){
	  buzzer_set_period(5000);
	  timer++;
	}
	newPos.axes[axis] += (2*velocity);

	buzzer_set_period(0);
	
      }	/**< if outside of fence */

      
      // check for collision with each ball
      /*   
      if(   (ml->layer->posNext.axes[0] == ml1->layer->posNext.axes[0])
	 && (ml->layer->posNext.axes[1] ==ml1->layer->posNext.axes[1])){

	int velocity = ml->velocity.axes[axis] = -ml->velocity.axes[axis];
	ml->velocity.axes[0]+=1;
	newPos.axes[axis] += (2*velocity);
      
      }
      
      */

    } /**< for axis */

    ml->layer->posNext = newPos;

   
  } /**< for ml */
}

void collisionCheck(){
  Vec2 position = (&player)->pos;

  MovLayer *currentBlock = &ml0;

  while(currentBlock){

    AbRect* block = (AbRect*)((currentBlock)->layer->abShape);

    Region blockBoundary;
    layerGetBounds((currentBlock->layer), &blockBoundary);
    
    if(intersection(&position, &blockBoundary)) {

	(&player)->color = COLOR_WHITE;
      }
  }
}

  u_char intersection(Vec2 *p, Region* r){

    if(p->axes[0] < r->topLeft.axes[0] ||
       p->axes[0] > r->botRight.axes[0]){
      return 0;
    }

    if(p->axes[1] < r->topLeft.axes[1] ||
       p->axes[1] > r->botRight.axes[1]){
      return 0;
    }
    return 1;
  }


u_int bgColor = COLOR_GREEN;     /**< The background color */
int redrawScreen = 1;           /**< Boolean for whether screen needs to be redrawn */

Region fieldFence;		/**< fence around playing field  */


/** Initializes everything, enables interrupts and green LED, 
 *  and handles the rendering for the screen
 */
char *display = "Score";
  int score = 0;
  char displayInt[20];
  
void main()
{
  P1DIR |= GREEN_LED;		/**< Green led on when CPU on */		
  P1OUT |= GREEN_LED;

  configureClocks();
  lcd_init();
  shapeInit();
  p2sw_init(15);
  buzzer_init();
  shapeInit();

  layerInit(&layer0);
  layerDraw(&layer0);


  layerGetBounds(&fieldLayer, &fieldFence);

  enableWDTInterrupts();      /**< enable periodic interrupt */
  or_sr(0x8);	              /**< GIE (enable interrupts) */

  Vec2 pos1 = {15,130};
  Vec2 pos2 = {60,130};
  Vec2 pos3 = {110,130};
  Vec2 pos4 = {16,76};
  Vec2 pos5 = {62,80};
  Vec2 pos6 = {110,80};
  Vec2 pos7 = {15,30};
  Vec2 pos8 = {62,30};
  Vec2 pos9 = {110,30};
  
  static  int col = 5;
  static int row = 2;

  for(;;) { 
    while (!redrawScreen) { /**< Pause CPU if screen doesn't need updating */
      P1OUT &= ~GREEN_LED;    /**< Green led off witHo CPU */
      or_sr(0x10);	      /**< CPU OFF */
    }
    P1OUT |= GREEN_LED;       /**< Green led on when CPU on */
    redrawScreen = 0;
    movLayerDraw(&ml0, &layer0);
    movLayerDraw(&movPlayer, &layer0);
    
    u_int buttons = p2sw_read(), i;

    itoa(score, displayInt,10);
    
    drawString5x7(10,150,display,COLOR_BLACK, COLOR_GREEN);

    drawString5x7(50,150,displayInt,COLOR_BLACK, COLOR_GREEN);

    //    drawString5x7(110,80,".",COLOR_BLACK, COLOR_GREEN);
  
    score++;

    //sw3 - right
    if(!(buttons & 8)){
      
      if (col ==1 && row ==1){
	movPlayer.layer->posNext.axes[0] += 50;
	clearScreen(COLOR_GREEN);
	col++;
      }else

      if (col == 2 && row ==1){
	
	movPlayer.layer->posNext.axes[0] += 50;
	clearScreen(COLOR_GREEN);
	col++;
      }else

      if (col == 3 && row ==1){
	
      }else
      if (col == 4 && row ==2){
	movPlayer.layer->posNext.axes[0] += 50;
	clearScreen(COLOR_GREEN);
	col++;

      }else 
      if (col == 5  && row ==2){
	
	movPlayer.layer->posNext.axes[0] += 50;
	clearScreen(COLOR_GREEN);
	col++;
      }else
      if (col == 6  && row ==2){
	
      }else

      if (col == 7  && row ==3){
	movPlayer.layer->posNext.axes[0] += 50;
	clearScreen(COLOR_GREEN);
	col++;
      }else
      if (col == 8  && row ==3){
	
	movPlayer.layer->posNext.axes[0] += 50;
	clearScreen(COLOR_GREEN);
	col++;
      }else
      if (col == 9  && row ==3){
	
      }
    }
    
    //sw2 - down
    if(!(buttons & 4)){
      if(row==1){
	
      }else

	if (row <4){
movPlayer.layer->posNext.axes[1] += 50;
	clearScreen(COLOR_GREEN);
	row--;
	col -=3;
	}

    }
    
    //sw1 - up
    if(!(buttons & 2)){
      if(row<3){
	movPlayer.layer->posNext.axes[1] -= 50;
	clearScreen(COLOR_GREEN);
	row++;
	col+=3;
      }else

	if (row == 3){

	}

    }


    //sw0 - left
    if(!(buttons & 1)){
      if (col ==1 && row ==1){
    
      }

      if (col == 2 && row ==1){
	
	movPlayer.layer->posNext.axes[0] -= 50;
	clearScreen(COLOR_GREEN);
	col--;
      }

      if (col == 3 && row ==1){
	
	movPlayer.layer->posNext.axes[0] -= 50;
	clearScreen(COLOR_GREEN);
    	col--;
      }
      if (col == 4 && row ==2){
	col = 4;

      }
      if (col == 5  && row ==2){
	
	movPlayer.layer->posNext.axes[0] -= 50;
	clearScreen(COLOR_GREEN);
	col--;
      }
      if (col == 6  && row ==2){
	
	movPlayer.layer->posNext.axes[0] -= 50;
	clearScreen(COLOR_GREEN);
	col--;
      }

      if (col == 7  && row ==3){
	movPlayer.layer->posNext.axes[0] -= 50;
	clearScreen(COLOR_GREEN);
    
      }
      if (col == 8  && row ==3){
	
	movPlayer.layer->posNext.axes[0] -= 50;
	clearScreen(COLOR_GREEN);
	col--;
      }
      if (col == 9  && row ==3){
	
	movPlayer.layer->posNext.axes[0] -= 50;
	clearScreen(COLOR_GREEN);
	col--;
      }
    }

    ///////////////////////////////////////////////////
    //collision
    
    if(col == 1){
      Layer *probeLayer;
      for(probeLayer = &layer0;probeLayer;probeLayer = probeLayer->next){
	if(abShapeCheck(probeLayer->abShape,&probeLayer->pos,&pos1)){

	  clearScreen(COLOR_BLACK);
	  bgColor = COLOR_BLACK;
	  while(1){
	    drawString5x7(30,150,"YOU'RE DEAD", COLOR_RED, COLOR_BLACK);
	  }
	}
      }
    }
    if(col == 2){

      Layer *probeLayer;
      for(probeLayer = &layer0;probeLayer;probeLayer = probeLayer->next){
	if(abShapeCheck(probeLayer->abShape,&probeLayer->pos,&pos2)){

	  clearScreen(COLOR_BLACK);
	  bgColor = COLOR_BLACK;
	  while(1){
	    drawString5x7(30,150,"YOU'RE DEAD", COLOR_RED, COLOR_BLACK);
	  }
	}
      }
    }
    if(col == 3){
      Layer *probeLayer;
      for(probeLayer = &layer0;probeLayer;probeLayer = probeLayer->next){
	if(abShapeCheck(probeLayer->abShape,&probeLayer->pos,&pos3)){

	  clearScreen(COLOR_BLACK);
	  bgColor = COLOR_BLACK;
	  while(1){
	    drawString5x7(30,150,"YOU'RE DEAD", COLOR_RED, COLOR_BLACK);
	  }
	}
      }
    }
    if(col == 4){
  Layer *probeLayer;
      for(probeLayer = &layer0;probeLayer;probeLayer = probeLayer->next){
	if(abShapeCheck(probeLayer->abShape,&probeLayer->pos,&pos4)){

	  clearScreen(COLOR_BLACK);
	  bgColor = COLOR_BLACK;
	  while(1){
	    drawString5x7(30,150,"YOU'RE DEAD", COLOR_RED, COLOR_BLACK);
	  }
	}
      }
    }
    if(col == 5){
  Layer *probeLayer;
      for(probeLayer = &layer0;probeLayer;probeLayer = probeLayer->next){
	if(abShapeCheck(probeLayer->abShape,&probeLayer->pos,&pos5)){

	  clearScreen(COLOR_BLACK);
	  bgColor = COLOR_BLACK;
	  while(1){
	    drawString5x7(30,150,"YOU'RE DEAD", COLOR_RED, COLOR_BLACK);
	  }
	}
      }
    }
    if(col == 6){
  Layer *probeLayer;
      for(probeLayer = &layer0;probeLayer;probeLayer = probeLayer->next){
	if(abShapeCheck(probeLayer->abShape,&probeLayer->pos,&pos6)){

	  clearScreen(COLOR_BLACK);
	  bgColor = COLOR_BLACK;
	  while(1){
	    drawString5x7(30,150,"YOU'RE DEAD", COLOR_RED, COLOR_BLACK);
	  }
	}
      }
    }
    if(col == 7){
Layer *probeLayer;
      for(probeLayer = &layer0;probeLayer;probeLayer = probeLayer->next){
	if(abShapeCheck(probeLayer->abShape,&probeLayer->pos,&pos7)){

	  clearScreen(COLOR_BLACK);
	  bgColor = COLOR_BLACK;
	  while(1){
	    drawString5x7(30,150,"YOU'RE DEAD", COLOR_RED, COLOR_BLACK);
	  }
	}
      }
    }
    if(col == 8){
Layer *probeLayer;
      for(probeLayer = &layer0;probeLayer;probeLayer = probeLayer->next){
	if(abShapeCheck(probeLayer->abShape,&probeLayer->pos,&pos8)){

	  clearScreen(COLOR_BLACK);
	  bgColor = COLOR_BLACK;
	  while(1){
	    drawString5x7(30,150,"YOU'RE DEAD", COLOR_RED, COLOR_BLACK);
	  }
	}
      }
    }
    if(col == 9){
Layer *probeLayer;
      for(probeLayer = &layer0;probeLayer;probeLayer = probeLayer->next){
	if(abShapeCheck(probeLayer->abShape,&probeLayer->pos,&pos9)){

	  clearScreen(COLOR_BLACK);
	  bgColor = COLOR_BLACK;
	  while(1){
	    drawString5x7(30,150,"YOU'RE DEAD", COLOR_RED, COLOR_BLACK);
	  }
	}
      }
    }


    
  }
}

/** Watchdog timer interrupt handler. 15 interrupts/sec */
void wdt_c_handler()
{
  static short count = 0;
  P1OUT |= GREEN_LED;		      /**< Green LED on when cpu on */
  count ++;
  if (count == 15) {
    mlAdvance(&ml0, &fieldFence);    

    if (p2sw_read())
      redrawScreen = 1;
    count = 0;
  } 
  P1OUT &= ~GREEN_LED;		    /**< Green LED off when cpu off */
}
