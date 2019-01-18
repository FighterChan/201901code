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

#define BUF_LEN 128
#define COM_NUM 16
#define COM_LEN 16
char cmd[COM_NUM][COM_LEN];

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

int do_cmd()
{

}

int readline(FILE *fp)
{
  int i;
  char outname[32];
  char *token;
  char *p;
  char buf[BUF_LEN];
  memset(buf,0,sizeof(buf));
  memset(outname,0,sizeof(outname));

  while(fgets(buf,sizeof(buf),fp))
    {
      i = 0;
      p = buf;
      while((token = strsep(&p," ")) != NULL)
        {
          sscanf(token, "%s", cmd[i]);
          i++;
        }
      do_cmd(i,cmd,outname);
      memset(buf,0,sizeof(buf));
      memset(cmd,0,sizeof(cmd));
    }
  return 0;
}
