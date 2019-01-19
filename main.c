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
#include "list.h"

char cmd[STR_LEN][STR_LEN];
int ctr_type = INPUT_FRAME_MODE;
struct parse *parser;

//LIST_HEAD(zone_head);
//LIST_HEAD(mem_head);
//LIST_HEAD(zoneset_head);
//LIST_HEAD(parse_head);

struct parse *
create_parse (void)
{
  struct parse *p;
  p = (struct parse *) malloc (sizeof(struct parse));
  if (p == NULL)
    {
      return NULL;
    }
  INIT_LIST_HEAD (&p->zoneset_head);
  return p;
}

void
create_zoneset (const char (*p)[STR_LEN], void *data)
{

  printf ("%s %s\n", p[0], p[1]);
  struct parse *parse;
  struct zoneset *pzoneset;
  pzoneset = (struct zoneset *) malloc (sizeof(struct zoneset));
  if (pzoneset == NULL)
    {
      return;
    }
  /* 初始化zone链表 */
  INIT_LIST_HEAD (&pzoneset->zone_head);
  /* 给zone赋值ID号 */
  pzoneset->zone_id = atoi (p[1]);
  /* 添加时未激活 */
  pzoneset->active = UNACTIVE;
  /* 把zoneset节点加入链表 */
  list_add_tail (&p->list, &parser->zoneset_head);
}

void
act_zoneset (const char (*p)[STR_LEN], void *data)
{
  printf ("%s %s %s %s\n", p[0], p[1], p[2], p[3]);
  struct zoneset *pset, *nset;
  list_for_each_safe(pset,nset,&parser->zoneset_head)
    {
      if (pset->id == atoi (p[2]))
        {
          pset->active = ACTIVE;
        }
      else
        {
          pset->active = UNACTIVE;
        }
    }
}

void
create_zone (const char (*p)[STR_LEN], void *data)
{
  printf ("%s %s\n", p[0], p[1]);
  struct zone *pzone;
  struct zoneset *pset, *nset;
  pzone = (struct zone *) malloc (sizeof(struct zone));
  if (pzone == NULL)
    {
      return;
    }
  /* 初始化member链表 */
  INIT_LIST_HEAD (&pzone->mem_head);
  /* 给zone赋值ID号 */
  pzone->id = atoi (p[1]);
  list_for_each_safe(pset,nset,&parser->zoneset_head)
    {
      /* 找到对应的zoneset */
      if (pset->id == (int *) data[0])
        {
          /* 将zone节点入链 */
          list_add_tail (&pzone->list, &pset->zone_head);
        }
    }
}

void
create_member (const char (*p)[STR_LEN], void *data)
{
  printf ("%s %s\n", p[0], p[1]);
  struct zone *pzone, *nzone;
  struct zoneset *pset, *nset;
  struct zone_mem *pmem;
  pmem = (struct zone_mem *) malloc (sizeof(struct zone_mem));
  if (pmem == NULL)
    {
      return;
    }
  strncpy (pmem->member, p[1], strlen (p[1]));
  list_for_each_safe(pset,nset,&parser->zoneset_head)
    {
      list_for_each_safe(pzone,nzone,&zone_head)
        {
        /* 找到对应的zoneset、zone*/
          if (pset->id == (int *)data[0] && pzone->id == (int *)data[1])
            {
              list_add_tail (&pmem->list, &pzone->mem_head);
            }
        }
    }
}

void
del_member (const char (*p)[STR_LEN], void *data)
{
  printf ("%s %s\n", p[0], p[1]);
}

void
no_zone (const char (*p)[STR_LEN], void *data)
{

}

void
add_zone (const char (*p)[STR_LEN], void *data)
{
  printf ("%s %s\n", p[0], p[1]);
}

void
del_zone (const char (*p)[STR_LEN], void *data)
{

}

void
nozone_set (const char (*p)[STR_LEN], void *data)
{

}

void
int_tunnel (const char (*p)[STR_LEN], void *data)
{

}

void
add_route (const char (*p)[STR_LEN], void *data)
{
  printf ("%s %s %s %s\n", p[0], p[1], p[2], p[3]);
}

void
del_route (const char (*p)[STR_LEN], void *data)
{

}

void
add_arp (const char (*p)[STR_LEN], void *data)
{

}

void
del_arp (const char (*p)[STR_LEN], void *data)
{

}

void
input_frame (const char (*p)[STR_LEN], void *data)
{

}

void
sip (const char (*p)[STR_LEN], void *data)
{
  if (ctr_type == INPUT_FRAME_MODE)
    {
      printf ("frame sip %s\n", p[2]);
    }
  else
    {
      printf ("int sip %s\n", p[2]);
    }
}

void
dip (const char (*p)[STR_LEN], void *data)
{
  if (ctr_type == INPUT_FRAME_MODE)
    {
      printf ("frame dip %s\n", p[2]);
    }
  else
    {
      printf ("int dip %s\n", p[2]);
    }
}

struct cmd_table cmd_tables[] =
  {
    { "zone", 0, create_zone },
    { "add-member", 0, create_member },
    { "del-member", 0, del_member },
    { "nozone", 0, no_zone },
    { "zoneset", 0, create_zoneset },
    { "add-zone", 0, add_zone },
    { "del-zone", 0, del_zone },
    { "nozoneset", 0, nozone_set },
    { "activezoneset", 0, act_zoneset },
    { "inttunnel", 0, int_tunnel },
    { "add-route", 0, add_route },
    { "del-route", 0, del_route },
    { "add-arp", 0, add_arp },
    { "del-arp", 0, del_arp },
    { "input-frame", 0, input_frame },
    { "sip=", 0, sip },
    { "dip=", 0, dip } };

void
parse_cmd (int sum, const char (*p)[STR_LEN])
{
  /* mode[0]:zoneset xxx
   * mode[1]:zone xxx
   * */
  int mode[2];
  int index;
  int i;
  int j;
  char command[2 * STR_LEN];
  index = 0;
  memset (command, 0, sizeof(command));
  memset (mode, 0, sizeof(mode));

  for (i = 0; i < (sum - 1); ++i)
    {
      sprintf (command + index, "%s", p[i]);
      index += strlen (p[i]);
    }

  for (j = 0; j < sizeof(cmd_tables) / sizeof(cmd_tables[0]); ++j)
    {
      /* 值有多个的情况 */
      if (strcmp (cmd_tables[j].cmd1, "add-arp") == 0
          || strcmp (cmd_tables[j].cmd1, "add-route") == 0)
        {
          if (strcmp (p[0], cmd_tables[j].cmd1) == 0)
            {
              cmd_tables[j].fun (p, &mode);
            }
        }
      else
        {
          if (strcmp (command, cmd_tables[j].cmd1) == 0)
            {
              /* 用来区分两种sip dip */
              if (strcmp (command, "inttunnel") == 0)
                {
                  ctr_type = GRE_MODE;
                }
              else if (strcmp (command, "input-frame") == 0)
                {
                  ctr_type = INPUT_FRAME_MODE;
                }
              else if (strcmp (command, "zoneset") == 0)
                {
                  mode[0] = atoi (p[1]);
                }
              else if (strcmp (command, "zone") == 0)
                {
                  mode[1] = atoi (p[1]);
                }
              cmd_tables[j].fun (p, (int *) mode);
            }
        }
    }
}

int
main (int argc, char **argv)
{
  parser = create_parse ();
  if (parse == NULL)
    {
      return -1;
    }
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
