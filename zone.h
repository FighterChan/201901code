/* /201901code/zone.h */
#ifndef _ZONE_H_
#define _ZONE_H_
#include "list.h"

#define STR_LEN     (32)

enum
{
  INPUT_FRAME_MODE, GRE_MODE
};

enum
{
  UNACTIVE, AVTIVE
};

enum
{
  ZONE,
  ADD_MEMBER,
  DEL_MEMBER,
  NO_ZONE,
  ZONESET,
  ADD_ZONE,
  DEL_ZONE,
  NO_ZONESET,
  ACTIVE_ZONESET,
  ADD_INT,
  NO_INT_TUNNEL,
  ADD_ROUTE,
  ADD_ARP,
  ACTIVE,
  INPUT_FRAME,
};

struct cmd_table
{
  char *cmd1;
  char mode;
  void
  (*fun) (const char (*cmd)[STR_LEN], void *data);
};

struct in_mess
{
  char sip[STR_LEN];
  char dip[STR_LEN];
  struct list_head list;
};

struct in_frame
{
  char sip[STR_LEN];
  char dip[STR_LEN];
  int id;
  struct list_head list;
};

struct ip_route_for
{
  char ip[STR_LEN];
  int v_int; /* 虚拟隧道接口 */
  struct list_head list;
};

struct arp
{
  char ip[STR_LEN];
  char mac[STR_LEN];
  char r_int[STR_LEN]; /* 真实出接口 */
  struct list_head list;
};

struct gre
{
  char sip[STR_LEN];
  char dip[STR_LEN];
  int id;
  struct list_head list;
};

struct int_tunnel
{
  int id;
  char sip[STR_LEN];
  char dip[STR_LEN];
  struct list_head list;
};

struct zone_mem
{
  char member[STR_LEN];
  int id;
  struct list_head list;
};

struct zone
{
  struct list_head mem_head;
  int id;
  int active; /* 當前設置區 */
  struct list_head list;
};

struct zoneset
{
  struct list_head zone_head;
  int id;
  int active; /* 激活时active = 1 */
  int active2; /* 表示當前設置區 */
  struct list_head list;
};

struct parse
{
  struct list_head zoneset_head;
  struct list_head zone_mode_head;
  struct list_head list;
};

/* body of zone.h */
#endif /* _ZONE_H_ */
