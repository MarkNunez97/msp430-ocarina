#ifndef include_abPlayer
#define include_abPlayer

typedef struct AbPlayer_s {
  void (*getBounds)(const struct AbPlayer_s *player, const Vec2 *centerPos, Region *bounds);
  int (*check)(const struct AbPlayer_s *shape, const Vec2 *centerPos, const Vec2 *pixelLoc);
  int size;
} AbPlayer;

/** As required by AbShape
 */
void abPlayerGetBounds(const AbPlayer *player, const Vec2 *centerPos, Region *bounds);

/** As required by AbShape
 */
int abPlayerCheck(const AbPlayer *player, const Vec2 *centerPos, const Vec2 *pixel);

#endif

