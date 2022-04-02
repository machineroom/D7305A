/* C source file "slave.c" */

#define  boss  2              /* Connection to be made to with the boss */

#include <stdio.h>
#include <assert.h>
#include <ctype.h>
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
    int         len, i;
    char old[] = "this text should not appear as it should be overwritten";
    char new [80];

    /* Check that the right number of channels have been passed */
    assert(inlen  > boss);
    assert(outlen > boss);

    in  = in_vec;
    out = out_vec;

    /* Read a length and then a pointer to a byte array */
    len = ChanInInt (in[boss]);
    ChanIn (in[boss], old, len);

    /* Convert vector to upper case in array "new" */
    /* Note that vector is not a C string as it has no /0 at the end */
    /* Occam could have declared the string as "string*#00" */

    for (i = 0; i < len; i++)
      new[i] = toupper(old[i]);  /* Convert to upper case */

    /* Return results by sending length and actual data (for variety) */

    ChanOutInt (out[boss], len);
    ChanOut (out[boss], new, len);   /* Terminate slave routine    */
                                     /* Ending main returns to occam */
}

/* End of file "slave.c" */
