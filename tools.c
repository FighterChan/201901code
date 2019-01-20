/* 
 * tools.c
 * Original Author: chenwei3@ruijie.com.cn, 2018-12-13
 *
 * 
 * 
 *
 * History 
 * 
 *  
 */
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "tools.h"

int
filename (char *infile, char *outfile)
{
  assert(infile != NULL);
  char *token;
  char *outpath = outfile;
  if ((token = strsep (&infile, ".")) != NULL)
    {
      sprintf (outpath, "%s%s", token, "_result.txt");
    }
  else
    {
      return -1;
    }
  return 0;
}
