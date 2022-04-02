#define  bouncer 2    /* Connection to be made to occam bouncer */

#include <stdio.h>
#include <assert.h>
#include <misc.h>
#include <channel.h>

Channel **in, **out;          /* Externally scoped for global reference */

int main (int argc, char* argv[],   /* As you would expect */
          char* envp[],       /* Environment pointers */
          Channel* in_vec[],  /* Used to connect channels from occam */
          int inlen,
          Channel* out_vec[], /* Used to connect channels to occam */
          int outlen)

{
    int         i;

    /* Check that the right number of channels have been passed */
    assert(inlen  > bouncer);
    assert(outlen > bouncer);

    in  = in_vec;
    out = out_vec;

    printf("\nC: Sending 10 numbers down a channel...\n");
    printf("\nC: I hope someone is on the other end!\n");

    for (i = 1; i < 10; ++i)
      {
         printf ("C: Sending %d", i);
         ChanOutInt (out[bouncer], i);
         printf ("  Received %d\n", ChanInInt (in[bouncer]));
      }

    printf("\nC: That was gratifying! Now terminate\n");
    ChanOutInt (out[bouncer], -1);  /* Terminate bouncer routine    */
                                    /* Ending main returns to occam */
}
