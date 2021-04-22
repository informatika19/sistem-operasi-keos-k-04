#include "math.h"
int div(int x, int y) {
  return x/y;
}

int mod(int x, int m) {
  while (x >= m) { x -= m; }
  return x;
}
