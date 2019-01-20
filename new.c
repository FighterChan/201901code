/* 
 * new.c
 * Original Author: chenwei3@ruijie.com.cn, 2019-01-20
 *
 * 
 * 
 *
 * History 
 * 
 *  
 */

#define MAX_ARGC 10

#define MAX_CMD_NUM 50

/* Monitor Command Prompt */
#define CONFIG_SYS_PROMPT "->"

/* Buffer size for input from the Console */
#define CONFIG_SYS_CBSIZE   256

int cmd_num_current = 0;


//命令结构体
typedef struct CMD_STRUCT
{
  char name[32]; /* Command Name */
  char usage[64];/* Usage message */
  void (*CmdFun)(int , char *[]);/* Command execute function */
}CMD_STRUCT_T;


//命令列表
CMD_STRUCT_T CmdTbl[MAX_CMD_NUM];


void read_fun(int agrc, char * argv [])
{
  int i = 0;

  while (i < cmd_num_current)
  {
    if (read_fun == CmdTbl[i].CmdFun)
    {
      printf("%s -- %s\n", CmdTbl[i].name,  CmdTbl[i].usage);
      break;
    }
    i++;
  }

  return;
}


int register_cmd(char *name, char *usage, cmd_fun_t fun)
{
  int ret;

  if (cmd_num_current < MAX_CMD_NUM)
  {
    strcpy(CmdTbl[cmd_num_current].name, name);
    strcpy(CmdTbl[cmd_num_current].usage , usage);

    CmdTbl[cmd_num_current].CmdFun = fun;

    cmd_num_current++;
  }
  else
  {
    printf("%s error\n");
    return 1;
  }

  return 0;
}

char parse_buf[256] ;
int parse_line(const char * const line, char *argv[])
{
  int argc = 0;

  char *ptr = parse_buf;
  memset(parse_buf, '\0', 256);
  strncpy(parse_buf, line, strlen(line));

  while ((argv[argc]=strtok(ptr, " "))!=NULL)
  {
//printf("argv[%d]:%s\n", argc, argv[argc]);
    argc++;

    if (argc > MAX_ARGC)
      break;
    ptr = NULL;
  }
  return argc;
}

