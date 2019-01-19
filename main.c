/* 
 * main.c
 * Original Author: chenwei3@ruijie.com.cn, 2019-01-18
 *
 * 
 * 
 *
 * History 
 * 
 *  

 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tools.h"
#include "zone.h"

char cmd[STR_LEN][STR_LEN];

void
zone (const char (*p)[STR_LEN], int num)
{
  printf ("%s %s\n", p[0], p[1]);
}

void
add_member (const char (*p)[STR_LEN], int num)
{
  printf ("%s %s\n", p[0], p[1]);
}

void
del_member (const char (*p)[STR_LEN], int num)
{
  printf ("%s %s\n", p[0], p[1]);
}

void
no_zone (const char (*p)[STR_LEN], int num)
{

}

void
zoneset (const char (*p)[STR_LEN], int num)
{

}

void
add_zone (const char (*p)[STR_LEN], int num)
{
  printf ("%s %s\n", p[0], p[1]);
}

void
del_zone (const char (*p)[STR_LEN], int num)
{

}

void
nozone_set (const char (*p)[STR_LEN], int num)
{

}

void
act_zoneset (const char (*p)[STR_LEN], int num)
{

}

void
int_tunnel (const char (*p)[STR_LEN], int num)
{

}

void
add_route (const char (*p)[STR_LEN], int num)
{

}

void
del_route (const char (*p)[STR_LEN], int num)
{

}

void
add_arp (const char (*p)[STR_LEN], int num)
{

}

void
del_arp (const char (*p)[STR_LEN], int num)
{

}

void
input_frame (const char (*p)[STR_LEN], int num)
{

}

void
sip (const char (*p)[STR_LEN], int num)
{

}

void
dip (const char (*p)[STR_LEN], int num)
{

}

struct cmd_table cmd_tables[] =
  {
    { "zone", ZONE, zone },
    { "add-member", ADD_MEMBER, add_member },
    { "del-member", DEL_MEMBER, del_member },
    { "nozone", NO_ZONE, no_zone },
    { "zoneset", ZONESET, zoneset },
    { "add-zone", ADD_ZONE, add_zone },
    { "del-zone", DEL_ZONE, del_zone },
    { "nozoneset", NO_ZONESET, nozone_set },
    { "activezoneset", ACTIVE_ZONESET, act_zoneset },
    { "int-tunnel", 0, int_tunnel },
    { "add-route", 0, add_route },
    { "del-route", 0, del_route },
    { "add-arp", 0, add_arp },
    { "del-arp", 0, del_arp },
    { "input-frame", 0, input_frame },
    { "sip", 0, sip },
    { "dip", 0, dip } };

void
parse_cmd (int sum, const char (*p)[STR_LEN])
{
  char type;
  int index;
  int i;
  int j;
  char command[2 * STR_LEN];
  index = 0;
  memset (command, 0, sizeof(command));
  for (i = 0; i < (sum - 1); ++i)
    {
      sprintf (command + index, "%s", p[i]);
      index += strlen (p[i]);
    }

  for (j = 0; j < sizeof(cmd_tables) / sizeof(cmd_tables[0]); ++j)
    {
      if (strcmp (cmd_tables[j].cmd1, "add-arp") == 0
          || strcmp (cmd_tables[j].cmd1, "add-route") == 0)
        {
          if (strcmp (p[0], cmd_tables[j].cmd1) == 0)
            {
              cmd_tables[j].fun (p, sum);
            }
        }
      else
        {
          if (strcmp (command, cmd_tables[j].cmd1) == 0)
            {
              cmd_tables[j].fun (p, sum);
            }
        }
    }
}

int
main (int argc, char **argv)
{

  char buf[128] =
    { 0 };
  FILE *fp;
  fp = fopen (argv[1], "r");
  if (fp == NULL)
    {
      return -1;
    }
  memset (cmd, 0, sizeof(cmd));
  char *token;
  char *p;
  int i;
  while (fgets (buf, sizeof(buf), fp))
    {

      i = 0;
      p = buf;
      while ((token = strsep (&p, " ")) != NULL)
        {
          sscanf (token, "%s", cmd[i]);
          i++;
        }
      parse_cmd (i, cmd);
      memset (buf, 0, sizeof(buf));
      memset (cmd, 0, sizeof(cmd));
    }
  return 0;
}
