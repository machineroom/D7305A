#include <channel.h>
#include <stdio.h>
#include <assert.h>

int main (int argc, char *argv[], char *envp[],
          Channel *in[],  int inlen,
          Channel *out[], int outlen)
{
  int x;

  /* Check that the right number of channels have been passed */
  assert(inlen  > 2);
  assert(outlen > 2);

  printf("This is the C program: start\n");

  ChanIn(in[2], &x, sizeof(x));
  printf("This is the C program: received x = %d\n", x);

  x = x * 2;
  printf("This is the C program: calculated x*2 = %d\n", x);

  ChanOut(out[2], &x, sizeof(x));
  printf("This is the C program: sent x*2\n");

  ChanOut(out[2], &x, sizeof(x)); /* synchronise the shutdown */

  return 0;
}
