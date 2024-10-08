#ifndef _SPRAY_SOURCE_
#define _SPRAY_SOURCE_

int pressure_check(float val, float high, float low) {
  if (low > val) return 1;
  if (high < val) return -1;
  return 0;
}

#endif