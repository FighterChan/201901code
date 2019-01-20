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

LIST_HEAD(tunnel_head);
LIST_HEAD(frame_head);
LIST_HEAD(route_head);
LIST_HEAD(arp_head);

LIST_HEAD(zoneset_head);
LIST_HEAD(zone_head);
LIST_HEAD(mem_head);

struct parse *
create_parse (void)
{
  struct parse *p;
  p = (struct parse *) malloc (sizeof(struct parse));
  if (p == NULL)
    {
      return NULL;
    }
  /* 设置默认链表 */
  INIT_LIST_HEAD (&p->zoneset_head);
  INIT_LIST_HEAD (&p->zone_mode_head);
  return p;
}

void
create_zoneset (const char (*p)[STR_LEN], void *data)
{

  printf ("%s %s\n", p[0], p[1]);
  struct zoneset *pzoneset;
  pzoneset = (struct zoneset *) malloc (sizeof(struct zoneset));
  if (pzoneset == NULL)
    {
      return;
    }
  /* 初始化zone链表 */
  INIT_LIST_HEAD (&pzoneset->zone_head);
  /* 给zoneset赋值ID号 */
  pzoneset->id = atoi (p[1]);
  /* 添加时未激活 */
  pzoneset->active = UNACTIVE;
  /* 把zoneset节点加入链表 */
  list_add_tail (&pzoneset->list, &zoneset_head);
}

void
act_zoneset (const char (*p)[STR_LEN], void *data)
{
  printf ("%s %s %s %s\n", p[0], p[1], p[2], p[3]);
  struct zoneset *pset, *nset;
  list_for_each_entry_safe(pset,nset,&parser->zoneset_head,list)
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
  pzone = (struct zone *) malloc (sizeof(struct zone));
  if (pzone == NULL)
    {
      return;
    }
  INIT_LIST_HEAD(&pzone->mem_head);
  pzone->id = atoi (p[1]);
  list_add_tail (&pzone->list, &zone_head);
}

void
create_member (const char (*p)[STR_LEN], void *data)
{
  printf ("%s %s\n", p[0], p[1]);
  struct zone *pzone, *nzone;
  struct zone_mem *pmem;
  pmem = (struct zone_mem *) malloc (sizeof(struct zone_mem));
  if (pmem == NULL)
    {
      return;
    }
  strncpy (pmem->member, p[1], strlen (p[1]));
  /* 找到上次的zone区域 */
  pzone = list_last_entry(&zone_head,struct zone,list);
  if(pzone == NULL)
    {
      return;
    }
  list_add_tail(&pmem->list,&pzone->mem_head);
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
  struct zoneset *pzoneset;
  struct zone *pzone;
  pzone = (struct zone *) malloc (sizeof(struct zone));
  if (pzone == NULL)
    {
      return;
    }
  pzone->id = atoi(p[1]);
  pzoneset = list_last_entry(&zoneset_head,struct zoneset,list);
  if(pzoneset == NULL)
    {
      return;
    }
  struct zone *pz,*nz;
  struct zone *tmp;
  list_for_each_entry_safe(pz,nz,&zone_head,list)
  {
    if(pzone->id == pz->id)
      {
        tmp = pz;
      }
  }
  if(tmp != NULL){
      list_add_tail(&tmp->list,&pzoneset->zone_head);
  }
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
  struct int_tunnel *pint;
  pint = (struct int_tunnel *) malloc (sizeof(struct int_tunnel));
  if (pint == NULL)
    {
      return;
    }
  pint->id = atoi (p[2]);
  strcpy(pint->sip,"eol");
  strcpy(pint->dip,"eol");
  list_add_tail (&pint->list, &tunnel_head);
}

void
add_route (const char (*p)[STR_LEN], void *data)
{
  printf ("%s %s %s %s\n", p[0], p[1], p[2], p[3]);
  struct ip_route_for *pr;
  pr = (struct ip_route_for *)malloc(sizeof(struct ip_route_for));
  if(pr == NULL)
    {
      return;
    }
  strcpy(pr->ip,p[1]);
  pr->v_int = atoi(p[3]);
  list_add_tail(&pr->list,&route_head);
}

void
del_route (const char (*p)[STR_LEN], void *data)
{


}

void
add_arp (const char (*p)[STR_LEN], void *data)
{
  struct arp *pa;
  pa = (struct arp *)malloc(sizeof(struct arp));
  if(pa == NULL)
    {
      return;
    }
  strcpy(pa->ip,p[1]);
  strcpy(pa->mac,p[2]);
  strcpy(pa->r_int,p[3]);

  list_add_tail(&pa->list,&arp_head);
}

void
del_arp (const char (*p)[STR_LEN], void *data)
{

}

void
input_frame (const char (*p)[STR_LEN], void *data)
{
  struct in_frame *pf;
  pf = (struct in_frame *) malloc (sizeof(struct in_frame));
  if (pf == NULL)
    {
      return;
    }
  pf->id = atoi (p[1]);
  strcpy(pf->sip,"eol");
  strcpy(pf->dip,"eol");
  list_add_tail (&pf->list, &frame_head);

}

void
sip (const char (*p)[STR_LEN], void *data)
{
  struct int_tunnel *pint, *nint;
  struct in_frame *pf,*nf;
  if (ctr_type == INPUT_FRAME_MODE)
    {
      printf ("frame sip %s\n", p[2]);
      list_for_each_entry_safe(pf,nf,&frame_head,list)
      {
        strcpy(pf->sip,p[2]);
      }
    }
  else
    {
      printf ("int sip %s\n", p[2]);
      list_for_each_entry_safe(pint,nint,&tunnel_head,list)
      {
        strcpy(pint->sip,p[2]);
      }
    }
}

void
dip (const char (*p)[STR_LEN], void *data)
{
  struct int_tunnel *pint, *nint;
  struct in_frame *pf,*nf;
  if (ctr_type == INPUT_FRAME_MODE)
    {
      printf ("frame dip %s\n", p[2]);
      list_for_each_entry_safe(pf,nf,&frame_head,list)
      {
        strcpy(pf->dip,p[2]);
      }
    }
  else
    {
      printf ("int dip %s\n", p[2]);
      list_for_each_entry_safe(pint,nint,&tunnel_head,list)
      {
        strcpy(pint->dip,p[2]);
      }
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

struct ip_route_for *
look_up_route_by_dip (const char *dip)
{
  struct ip_route_for *p, *n;
  list_for_each_entry_safe (p, n, &route_head,list)
    {
      if (strcmp(p->ip,dip) == 0)
        {
          return p;
        }
    }
  return NULL;
}

struct int_tunnel *
look_up_tunnel_dip_by_route_vint (int route_vint)
{
  struct int_tunnel *p, *n;
  list_for_each_entry_safe (p, n, &tunnel_head,list)
    {
      if (p->id == route_vint)
        {
          return p;
        }
    }
  return NULL;
}

struct arp *
look_up_arp_rint_by_tunnel_dip (const char *tunnel_dip)
{
  struct arp *p, *n;
  list_for_each_entry_safe (p, n, &arp_head,list)
    {
      if (strcmp(p->ip,tunnel_dip) == 0)
        {
          return p;
        }
    }
  return NULL;
}



void output_file(void)
{
  struct in_frame *pf,*nf;
  struct ip_route_for *pr,*nr;
  struct int_tunnel *pi,*ni;
  struct arp *pa,*na;
  /*查找輸入表*/
  list_for_each_entry_safe(pf,nf,&frame_head,list)
  {
    printf("%s\n",pf->dip);
    pr = look_up_route_by_dip(pf->dip);
    if(pr == NULL)
      {
        printf("pr NULL\n");
        return;
      }
    pi = look_up_tunnel_dip_by_route_vint(pr->v_int);
    if(pi == NULL)
      {
        printf("pi NULL\n");
        return;
      }
    pa = look_up_arp_rint_by_tunnel_dip(pi->dip);
    if(pa == NULL)
      {
        printf("pa NULL\n");
        return;
      }
    printf("%s\n",pa->r_int);
  }
}

int
main (int argc, char **argv)
{
  parser = create_parse ();
  if (parser == NULL)
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
  output_file();
  return 0;
}
