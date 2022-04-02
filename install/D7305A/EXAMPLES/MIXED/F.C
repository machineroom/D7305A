/*static int z = 99;*/
#include <stdlib.h>
int f(int x)
{
  int y ;
/*  int *p = (int *)malloc(sizeof(int));
  *p = 1;
  free(p); */
  y = x + 1;
  /*z = y;*/
  return y;
}
