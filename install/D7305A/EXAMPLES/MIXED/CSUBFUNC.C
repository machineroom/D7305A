#include <stdlib.h>
static int *c;
int c_func(int x, int check)
{
  int i;

  if (check == 0)
  {
    c = (int *)malloc(x);
    if (c == NULL)
      return 1;
    else
    {
      for (i=0; i < x / sizeof(int); i++)
        c[i] = i;
    }
  }
  else
  {
    for (i=0; i < x / sizeof(int); i++)
      if (c[i] != i)
        return 1;
   }
   return 0;
}
