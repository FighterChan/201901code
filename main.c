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


int main(int argc, char **argv) {

  char buf[32] = {0};
  filename(argv[1],buf);
  printf("%s\n",buf);
  return 0;
}
