/* /201901code/zone.h */
#ifndef _ZONE_H_
#define _ZONE_H_
#include "list.h"

#define STR_LEN     (32)

enum {
  INPUT_FRAME_MODE,
  GRE_MODE
};

enum {
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

struct cmd_table {
  char *cmd1;
  char mode;
  void (*fun)(const char (*cmd)[STR_LEN],int type);
};

struct in_mess {
  char sip[STR_LEN];
  char dip[STR_LEN];
  struct list_head list;
};

struct ip_for {
  char mac[STR_LEN];
  char v_int[STR_LEN]; /* 虚拟隧道接口 */
  struct list_head list;
};

struct arp {
  char ip[STR_LEN];
  char mac[STR_LEN];
  char r_int[STR_LEN]; /* 真实出接口 */
  struct list_head list;
};

struct gre {
  char sip[STR_LEN];
  char dip[STR_LEN];
  struct list_head list;
};

struct zone_mem {
  char member[STR_LEN];
  struct list_head list;
};

struct zone {
  struct zone_mem mem_body;
  int zone_id;
  struct list_head list;
};

struct zoneset {
  struct zone zone_body;
  int zoneset_id;
  int active; /* 激活时active = 1 */
  struct list_head list;
};

struct parse {

};

/* body of zone.h */
#endif /* _ZONE_H_ */
