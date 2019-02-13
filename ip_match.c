/* 
 * ip_match.c
 * Original Author: chenwei3@ruijie.com.cn, 2019-01-31
 *
 * 
 * 
 *
 * History 
 * 
 *  

 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "ip_match.h"

/* Maskbit. */
static const unsigned char maskbit[] =
  { 0x00, 0x80, 0xc0, 0xe0, 0xf0, 0xf8, 0xfc, 0xfe, 0xff };

/* When string format is invalid return 0. */
/* else return 1 */
int
str2prefix_ipv4 (const char *str, struct prefix_ipv4 *p)
{
  unsigned int network;
  int plen;
  int ret;
  size_t alloc;
  char *pnt;
  char *cp;

  /* Find slash inside string. */
  pnt = strchr (str, '/');

  /* String doesn't contail slash. */
  if (pnt == NULL)
    {
      printf ("pnt == NULL :  %s\n", str);
      /* Convert string to prefix. */
      ret = inet_pton (AF_INET, str, &p->prefix);
      if (ret <= 0)
        return 0;
      p->family = AF_INET;

      /* Natural Mask is derived for network without a mask */
      network = ntohl (p->prefix.s_addr);
      p->prefix.s_addr = network;
      printf ("inet_pton network = %x\n", p->prefix.s_addr);
      if (p->prefix.s_addr  == 0)
        {
          p->prefixlen = 0;
          return 1;
        }

      if (IN_CLASSA(network))
        {
          if ((network & IN_CLASSA_NET) == network)
            p->prefixlen = IN_CLASSA_PREFIXLEN;
          else
            p->prefixlen = IPV4_MAX_PREFIXLEN;
        }
      else if (IN_CLASSB(network))
        {
          if ((network & IN_CLASSB_NET) == network)
            p->prefixlen = IN_CLASSB_PREFIXLEN;
          else
            p->prefixlen = IPV4_MAX_PREFIXLEN;
        }
      else if (IN_CLASSC(network))
        {
          if ((network & IN_CLASSC_NET) == network)
            p->prefixlen = IN_CLASSC_PREFIXLEN;
          else
            p->prefixlen = IPV4_MAX_PREFIXLEN;
        }
      else if (IN_CLASSD(network))
        p->prefixlen = IPV4_MAX_PREFIXLEN;
      else if (IN_EXPERIMENTAL(network))
        return 0;
      else
        return 0;
      printf ("p->prefixlen = %d\n", p->prefixlen);
      return 1;
    }
  else
    {
      printf ("pnt != NULL :  %s\n", str);

      alloc = (pnt - str) + 1;
      cp = (char *) malloc (alloc);
      if (cp == NULL)
        return 0;

      strncpy (cp, str, pnt - str);
      *(cp + (pnt - str)) = '\0';
      printf ("cp:%s\n", cp);
      ret = inet_pton (AF_INET, cp, &p->prefix);
      free (cp);

      if (ret <= 0)
        return 0;
      network = ntohl (p->prefix.s_addr);
      p->prefix.s_addr = network;
      printf ("inet_pton network = %x\n", p->prefix.s_addr);
      /* Get prefix length. */
      plen = (unsigned char) strtoul (++pnt, NULL, 10);
      if (plen > 32)
        return 0;
      p->family = AF_INET;
      p->prefixlen = (unsigned char) plen;
      printf ("p->prefixlen = %d\n", p->prefixlen);
    }

  return 1;
}

/* When both prefix structure is not same, but will be same after
 applying mask, return 0. otherwise, return 1 */
int
prefix_cmp (struct prefix_ipv4 *p1, struct prefix_ipv4 *p2)
{
  int offset;
  int shift;

  /* Set both prefix's head pointer. */
  unsigned char *pp1 = (unsigned char *) &p1->prefix;
  unsigned char *pp2 = (unsigned char *) &p2->prefix;

  if (p1->prefixlen != p2->prefixlen)
    return 1;

  offset = p1->prefixlen / 8;
  shift = p1->prefixlen % 8;

  if (shift)
    if (maskbit[shift] & (pp1[offset] ^ pp2[offset]))
      return 1;

  while (offset--)
    if (pp1[offset] != pp2[offset])
      return 1;

  return 0;
}
