/* /mycode/201901code/ip_match.h */
#ifndef _201901CODE_IP_MATCH_H_
#define _201901CODE_IP_MATCH_H_


#include <netinet/in.h>
/* IPv4 prefix structure. */
struct prefix_ipv4
{
  u_int8_t family;
  u_int8_t prefixlen;
  u_int8_t pad1;
  u_int8_t pad2;
  struct in_addr prefix;
};

#define IN_CLASSA_PREFIXLEN    8
#define IN_CLASSB_PREFIXLEN    16
#define IN_CLASSC_PREFIXLEN    24
#define IPV4_MAX_PREFIXLEN 32
int
str2prefix_ipv4 (const char *str, struct prefix_ipv4 *p);
int
prefix_cmp (struct prefix_ipv4 *p1, struct prefix_ipv4 *p2);

/* body of ip_match.h */
#endif /* _201901CODE_IP_MATCH_H_ */
