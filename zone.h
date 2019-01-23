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
  UNACTIVE, ACTIVE
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
  INPUT_FRAME,
};

struct cmd_table
{
  char *cmd1;
  char mode;
  void
  (*fun) (const char (*cmd)[STR_LEN], void *data);
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

struct int_tunnel
{
  char sip[STR_LEN];
  char dip[STR_LEN];
  int id;
  struct list_head list;
};

struct zone_mem
{
  char member[STR_LEN];
  struct list_head list;
};

struct zone
{
  int id;
  struct list_head mem_head;
  struct list_head list;
};

struct zoneset
{
  struct list_head zone_head;
  int id;
  int active; /* 激活时active = 1 */
  struct list_head list;
};

/* body of zone.h */
#endif /* _ZONE_H_ */
