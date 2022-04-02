#include <channel.h>
#include <assert.h>

int main (int argc, char *argv[], char *envp[],
          Channel *in[],  int inlen,
          Channel *out[], int outlen)
{
  int x;

  /* Check that the right number of channels have been passed */
  assert(inlen  > 2);
  assert(outlen > 2);

  ChanIn(in[2], &x, sizeof(x));
  x = x * 2;
  ChanOut(out[2], &x, sizeof(x));

  ChanOut(out[2], &x, sizeof(x)); /* synchronise the shutdown */

  return 0;
}

