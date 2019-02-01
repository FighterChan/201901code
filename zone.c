/* 
 * zone.c
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
#include <arpa/inet.h>
#include "zone.h"
#include "list.h"
#include "ip_match.h"

char cmd[STR_LEN][STR_LEN];
int ctr_type = INPUT_FRAME_MODE;

LIST_HEAD(tunnel_head);
LIST_HEAD(frame_head);
LIST_HEAD(route_head);
LIST_HEAD(arp_head);

LIST_HEAD(zoneset_head);
LIST_HEAD(zone_head);

#define SIZE 128
/* 字符串获取字节序 */
static int
get_network_byte (char *s)
{
  char buf[SIZE];
  struct in_addr p;

  strcpy (buf, s);
  inet_pton (AF_INET, buf, (void *) &p);

  return htonl (p.s_addr);
}

static int
match_address (const char *prefix, const char *dip, int offset)
{

  char buf_pre[SIZE], buf_dip[SIZE], buf1[SIZE], buf2[SIZE];
  unsigned int n_pre, n_dip, n;

  strcpy (buf_pre, prefix);
  strcpy (buf_dip, dip);
  n_pre = get_network_byte (buf_pre);
  n_dip = get_network_byte (buf_dip);
  n = n_dip - n_pre;
  if (n > 0 && n < (1 << offset))
    {
      return 1;
    }

  return 0;
}

int
filename (char *infile, char *outfile)
{
  if (infile == NULL || outfile == NULL)
    {
      return -1;
    }
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

/* 建立一个zone节点，并初始化挂在该zone下的一个member链表 */
void
create_zone (const char (*p)[STR_LEN], void *data)
{

  struct zone *pzone;
  pzone = (struct zone *) malloc (sizeof(struct zone));
  if (pzone == NULL)
    {
      return;
    }
  pzone->id = atoi (p[1]);
  INIT_LIST_HEAD (&pzone->mem_head);
  list_add_tail (&pzone->list, &zone_head);
}

/* 建立一个member节点 */
void
add_member (const char (*p)[STR_LEN], void *data)
{
  struct zone_mem *pmem;
  pmem = (struct zone_mem *) malloc (sizeof(struct zone_mem));
  if (pmem == NULL)
    {
      return;
    }
  strncpy (pmem->member, p[1], strlen (p[1]));
  struct zone *pzone;
  /* 找到最后插入的zone节点 */
  pzone = list_last_entry(&zone_head, struct zone, list);
  if (pzone == NULL)
    {
      return;
    }
  list_add_tail (&pmem->list, &pzone->mem_head);
}

void
create_zoneset (const char (*p)[STR_LEN], void *data)
{
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
add_zone (const char (*p)[STR_LEN], void *data)
{
  int id;
  id = atoi (p[1]);
  struct zoneset *pset;
  pset = list_last_entry(&zoneset_head, struct zoneset, list);
  if (pset == NULL)
    {
      return;
    }
  struct zone *newzone;
  struct zone *pzone, *nzone;
  list_for_each_entry_safe(pzone,nzone,&zone_head,list)
    {
      /* 找到匹配的zone */
      if (pzone->id == id)
        {
          printf ("匹配 id = %d\n", id);
          newzone = (struct zone *) malloc (sizeof(struct zone));
          if (newzone == NULL)
            {
              return;
            }
          memcpy (newzone, pzone, sizeof(struct zone));
          printf ("pzone->mem_head.next %p\n", pzone->mem_head.next);
          printf ("newzone->mem_head.next %p\n", newzone->mem_head.next);
          INIT_LIST_HEAD (&newzone->mem_head);
          struct zone_mem *pmem;
          struct zone_mem *pm;
          list_for_each_entry(pm,&pzone->mem_head,list)
            {
              pmem = (struct zone_mem *) malloc (sizeof(struct zone_mem));
              if (pmem == NULL)
                {
                  return;
                }
              memcpy (pmem, pm, sizeof(struct zone_mem));
              list_add_tail (&pmem->list, &newzone->mem_head);
            }
        }
    }
  list_add_tail (&newzone->list, &pset->zone_head);
}

int
check_frame_in (const char *sip, const char *dip)
{
  /* 暂时关闭这个函数 */
//  return 0;
  printf ("\nstart check frame in...\n");
  struct zoneset *pset, *nset;
  struct zone *pz, *nz;
  struct zone_mem *pmem, *nmem;
  int id1 = 0;
  int id2 = 0;
  int lock1 = 0;
  int lock2 = 0;

  list_for_each_entry_safe(pset,nset,&zoneset_head,list)
    {
      /*一次只会生效一个zoneset,因此，ACTIVE就是唯一的*/
      if (pset->active == ACTIVE)
        {
          printf ("zoneset id = %d\n", pset->id);
          list_for_each_entry_safe(pz,nz,&pset->zone_head,list)
            {

              printf ("zone id = %d\n", pz->id);

              list_for_each_entry_safe(pmem,nmem,&pz->mem_head,list)
                {
                  if (lock1 == 0 && strcmp (sip, pmem->member) == 0)
                    {
                      printf ("sip same = %s\n", sip);
                      lock1 = 1;
                      id1 = pz->id;
                    }
                  if (lock2 == 0 && strcmp (dip, pmem->member) == 0)
                    {
                      printf ("dip same = %s\n", dip);
                      lock2 = 1;
                      id2 = pz->id;
                    }
                  if (lock1 == 1 && lock2 == 1 && id1 == id2)
                    {
                      return 0;
                    }
                }
            }
        }
    }
  return -1;
}

void
act_zoneset (const char (*p)[STR_LEN], void *data)
{

  struct zoneset *pset, *nset;
  list_for_each_entry_safe(pset,nset,&zoneset_head,list)
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
del_member (const char (*p)[STR_LEN], void *data)
{

}

void
no_zone (const char (*p)[STR_LEN], void *data)
{

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
  strcpy (pint->sip, "eol");
  strcpy (pint->dip, "eol");
  list_add_tail (&pint->list, &tunnel_head);
}

void
add_route (const char (*p)[STR_LEN], void *data)
{

  struct ip_route_for *pr;
  pr = (struct ip_route_for *) malloc (sizeof(struct ip_route_for));
  if (pr == NULL)
    {
      return;
    }
  strcpy (pr->ip, p[1]);
  pr->v_int = atoi (p[3]);
  list_add_tail (&pr->list, &route_head);
}

void
del_route (const char (*p)[STR_LEN], void *data)
{

}

void
add_arp (const char (*p)[STR_LEN], void *data)
{

  struct arp *pa;
  pa = (struct arp *) malloc (sizeof(struct arp));
  if (pa == NULL)
    {
      return;
    }
  strcpy (pa->ip, p[1]);
  strcpy (pa->mac, p[2]);
  strcpy (pa->r_int, p[3]);

  list_add_tail (&pa->list, &arp_head);
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
  strcpy (pf->sip, "eol");
  strcpy (pf->dip, "eol");
  list_add_tail (&pf->list, &frame_head);

}

void
sip (const char (*p)[STR_LEN], void *data)
{

  struct int_tunnel *pint, *nint;
  struct in_frame *pf, *nf;
  if (ctr_type == INPUT_FRAME_MODE)
    {
      pf = list_last_entry(&frame_head, struct in_frame, list);
      if (pf == NULL)
        {
          return;
        }
      strcpy (pf->sip, p[2]);

    }
  else
    {
      pint = list_last_entry(&tunnel_head, struct int_tunnel, list);
      if (pint == NULL)
        {
          return;
        }
      if (strcmp (pint->sip, p[2]) == 0)
        {
          /*eol 表示sip为空*/
          strcpy (pint->sip, "eol");
        }
      else
        {
          strcpy (pint->sip, p[2]);
        }
    }
}

void
dip (const char (*p)[STR_LEN], void *data)
{

  struct int_tunnel *pint, *nint;
  struct in_frame *pf, *nf;
  if (ctr_type == INPUT_FRAME_MODE)
    {
      pf = list_last_entry(&frame_head, struct in_frame, list);
      if (pf == NULL)
        {
          return;
        }
      strcpy (pf->dip, p[2]);
    }
  else
    {
      pint = list_last_entry(&tunnel_head, struct int_tunnel, list);
      if (pint == NULL)
        {
          return;
        }
      if (strcmp (pint->dip, p[2]) == 0)
        {
          strcpy (pint->dip, "eol");
        }
      else
        {
          strcpy (pint->dip, p[2]);
        }
    }
}

struct cmd_table cmd_tables[] =
  {
    { "zone", 0, create_zone },
    { "add-member", 0, add_member },
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
#if 1
  printf ("%s %s %s %s\n", p[0], p[1], p[2], p[3]);
#endif

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
  char *token;
  char *ps;
  int mask = 0;
  int i;
  struct prefix prefix1,prefix2;
  memset (&prefix1, 0, sizeof(struct prefix));
  memset (&prefix2, 0, sizeof(struct prefix));

  list_for_each_entry_safe (p, n, &route_head,list)
    {
      str2prefix_ipv4(p->ip,&prefix1);
      str2prefix_ipv4(dip,&prefix2);

      if (prefix_cmp (&prefix1, &prefix2) == 0)
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
      if (p->id == route_vint && strcmp (p->dip, "eol") != 0
          && strcmp (p->sip, "eol") != 0)
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
      if (strcmp (p->ip, tunnel_dip) == 0)
        {
          return p;
        }
    }
  return NULL;
}

void
output_file (char *path)
{
  FILE *fp;
  struct in_frame *pf, *nf;
  struct ip_route_for *pr, *nr;
  struct int_tunnel *pi, *ni;
  struct arp *pa, *na;
  char outpath[32];
  memset (outpath, 0, sizeof(outpath));
  filename (path, outpath);
  fp = fopen (outpath, "w");
  if (fp == NULL)
    {
      return;
    }
  /*查找輸入表*/
  list_for_each_entry_safe(pf,nf,&frame_head,list)
    {
      printf ("frame in sip ->%s\n", pf->sip);
      printf ("frame in dip ->%s\n", pf->dip);
      /* 检查frame in 是否合法 */
      if (check_frame_in (pf->sip, pf->dip) < 0)
        {
          printf ("frame in error\n");
          printf ("\t=> frame %d DROP\n", pf->id);
          fprintf (fp, "%s %d %s\n", "frame", pf->id, "DROP");
          continue;
        }
      /* 通过输入表项dip查找ip转发表 */
      pr = look_up_route_by_dip (pf->dip);
      if (pr == NULL)
        {
          printf ("pr NULL\n");
          printf ("\t=> frame %d DROP\n", pf->id);
          fprintf (fp, "%s %d %s\n", "frame", pf->id, "DROP");
          continue;
        }
      /* 通过ip转发表的虚拟接口查找隧道dip */
      pi = look_up_tunnel_dip_by_route_vint (pr->v_int);
      if (pi == NULL)
        {
          printf ("pi NULL\n");
          fprintf (fp, "%s %d %s\n", "frame", pf->id, "DROP");
          continue;
        }
      /* 通过隧道dip查找arp表的真实出接口 */
      pa = look_up_arp_rint_by_tunnel_dip (pi->dip);
      if (pa == NULL)
        {
          printf ("pa NULL\n");
          fprintf (fp, "%s %d %s\n", "frame", pf->id, "DROP");
          continue;
        }

      printf ("\t=> frame %d %s\n", pf->id, pa->r_int);
      fprintf (fp, "%s %d %s\n", "frame", pf->id, pa->r_int);

    }
  fclose (fp);
}

void
free_all (void)
{
  struct zoneset *pset, *nset;
  struct zone *pz, *nz;
  struct zone_mem *pmem, *nmem;
  struct int_tunnel *pint, *nint;
  struct ip_route_for *pi, *ni;
  struct arp *pa, *na;
  struct in_frame *pf, *nf;

  list_for_each_entry_safe(pf,nf,&frame_head,list)
    {
      list_del_init (&pf->list);
      free (pf);
      pf = NULL;
    }

  list_for_each_entry_safe(pa,na,&arp_head,list)
    {
      list_del_init (&pa->list);
      free (pa);
      pa = NULL;
    }

  list_for_each_entry_safe(pi,ni,&route_head,list)
    {
      list_del_init (&pi->list);
      free (pi);
      pi = NULL;
    }

  list_for_each_entry_safe(pint,nint,&tunnel_head,list)
    {
      list_del_init (&pint->list);
      free (pint);
      pint = NULL;
    }

  list_for_each_entry_safe(pz,nz,&zone_head,list)
    {
      list_del_init (&pz->list);
      free (pz);
      pz = NULL;
    }

  pz = NULL;
  nz = NULL;
  list_for_each_entry_safe(pset,pset,&zoneset_head,list)
    {
      list_for_each_entry_safe(pz,nz,&pset->zone_head,list)
        {
          list_for_each_entry_safe(pmem,nmem,&pz->mem_head,list)
            {
              list_del_init (&pmem->list);
              free (pmem);
              pmem = NULL;
            }
          list_del_init (&pz->list);
          free (pz);
          pz = NULL;
        }

      list_del_init (&pset->list);
      free (pset);
      pset = NULL;
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
  fclose (fp);
  output_file (argv[1]);
//  free_all ();
  return 0;
}
