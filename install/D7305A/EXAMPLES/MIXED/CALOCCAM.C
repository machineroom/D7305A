#include <stdio.h>

extern int ocfunc (int arg1, int arg2);
/* declare function as extern */

#pragma IMS_nolink(ocfunc)
/* direct function to be compiled with no static base parameter */

static void call_occam(void)
{
  int arg1, arg2, ret;
  printf ("Calling occam function from C: ");
  arg1 = 0x7fffffff; arg2 = 4; /* set up arguments */

  ret = ocfunc (arg1, arg2);   /* call function */
  printf ("%d TIMES %d is %d\n", arg1, arg2, ret);
}

int main()
{
  call_occam();
}
