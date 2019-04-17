#include "shape.h"



int
abPlayerCheck(const AbPlayer *player, const Vec2 * centerPos, const Vec2 *pixel)
{
  Vec2 relPos;
  int row, col, within = 0;
  int size = player->size;
  int halfSize = size/2, quarterSize = halfSize/2;;
  vec2Sub(&relPos, pixel, centerPos);

  row = relPos.axes[1]; col = -relPos.axes[0];
  row = (row>=0)? row : -row;

  if (col>= 0){
    if(col<=quarterSize){
      within = row <= quarterSize;
    }else if(col <=halfSize){
      if(row>=quarterSize){
      within = row <= halfSize;
      }
    }else if(col <= halfSize+quarterSize){
       if(row>=halfSize){
      within = row <= halfSize;
      }
    }else if (col<= size){
      within = 0;
    }
  }
  
  return within;
}

void
abPlayerGetBounds(const AbPlayer *player, const Vec2 *centerPos, Region *bounds)
{
  int size = player-> size;
  bounds -> topLeft.axes[0] = centerPos->axes[0] - size;
  bounds -> topLeft.axes[1] = centerPos->axes[1] - size;
  bounds -> botRight.axes[0] = centerPos->axes[0];
  bounds -> botRight.axes[1] = centerPos->axes[1] + size;
}
