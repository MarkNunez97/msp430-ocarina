#include <libTimer.h>
#include <lcdutils.h>
#include <lcddraw.h>
#include "abCircle.h"

AbRect rect10 = {abRectGetBounds, abRectCheck, {2,9}};; /**< 10x10 rectangle */

AbRect rect11 = {abRectGetBounds, abRectCheck, {1,3}};; /**< 10x10 rectangle */

AbRect rect12 = {abRectGetBounds, abRectCheck, {1,1}};; /**< 10x10 rectangle */

AbRect rect13 = {abRectGetBounds, abRectCheck, {1,6}};; /**< 10x10 rectangle */


AbPlayer player1 = {abPlayerGetBounds, abPlayerCheck, 15};

u_int bgColor = COLOR_GREEN;

Layer body = {		/**< Layer with a red square */
  (AbShape *)&rect10,
  {screenWidth/2, screenHeight/2}, /**< center */
  {0,0}, {0,0},				    /* next & last pos */
  COLOR_RED,
  0,
};

Layer center = {		/**< Layer with an orange circle */
  (AbShape *)&circle6,
  {(screenWidth/2), (screenHeight/2)+2}, /**< bit below & right of center */
  {0,0}, {0,0},				    /* next & last pos */
  COLOR_ORANGE,
  &body,
};

Layer tl_wheel = {		/**< Layer with a red square */
  (AbShape *)&rect11,
  {(screenWidth/2)-5, (screenHeight/2)-6}, /**< center */
  {0,0}, {0,0},				    /* next & last pos */
  COLOR_BLACK,
  &center,
};


Layer tr_wheel = {		/**< Layer with a red square */
  (AbShape *)&rect11,
  {(screenWidth/2)+5, (screenHeight/2)-6}, /**< center */
  {0,0}, {0,0},				    /* next & last pos */
  COLOR_BLACK,
  &tl_wheel,
};


Layer bl_wheel = {		/**< Layer with a red square */
  (AbShape *)&rect11,
  {(screenWidth/2)-6, (screenHeight/2)+6}, /**< center */
  {0,0}, {0,0},				    /* next & last pos */
  COLOR_BLACK,
  &tr_wheel,
};


Layer br_wheel = {		/**< Layer with a red square */
  (AbShape *)&rect11,
  {(screenWidth/2)+6, (screenHeight/2)+6}, /**< center */
  {0,0}, {0,0},				    /* next & last pos */
  COLOR_BLACK,
  &bl_wheel,
};
Layer tr_dot = {		/**< Layer with a red square */
  (AbShape *)&rect12,
  {(screenWidth/2)+3, (screenHeight/2)-6}, /**< center */
  {0,0}, {0,0},				    /* next & last pos */
  COLOR_GRAY,
  &br_wheel,
};
Layer tl_dot = {		/**< Layer with a red square */
  (AbShape *)&rect12,
  {(screenWidth/2)-3, (screenHeight/2)-6}, /**< center */
  {0,0}, {0,0},				    /* next & last pos */
  COLOR_GRAY,
  &tr_dot,
};
Layer bl_dot = {		/**< Layer with a red square */
  (AbShape *)&rect12,
  {(screenWidth/2)-4, (screenHeight/2)+7}, /**< center */
  {0,0}, {0,0},				    /* next & last pos */
  COLOR_GRAY,
  &tl_dot,
};
Layer br_dot = {		/**< Layer with a red square */
  (AbShape *)&rect12,
  {(screenWidth/2)+4, (screenHeight/2)+7}, /**< center */
  {0,0}, {0,0},				    /* next & last pos */
  COLOR_GRAY,
  &bl_dot,
};
Layer car = {		/**< Layer with a red square */
  (AbShape *)&rect13,
  {(screenWidth/2), (screenHeight/2)+2}, /**< center */
  {0,0}, {0,0},				    /* next & last pos */
  COLOR_RED,
  &br_dot,
};

Layer player = {		/**< Layer with a red square */
  (AbShape *)&player1,
  {(screenWidth/2)+20, (screenHeight/2)+20}, /**< center */
  {0,0}, {0,0},				    /* next & last pos */
  COLOR_BLACK,
  0,
};

int
main()
{
  configureClocks();
  lcd_init();

  clearScreen(COLOR_BLUE);
  drawString5x7(20,20, "hello", COLOR_GREEN, COLOR_RED);

  layerDraw(&car);
  layerDraw(&player);

}
