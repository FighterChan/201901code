/* 
 * test.c
 * Original Author: chenwei3@ruijie.com.cn, 2019-01-20
 *
 * 
 * 
 *
 * History 
 * 
 *  
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


/*解析器当前状态*/
enum ParserState {
    INITIAL_STATE, /* 初始状态 */
    PARSING_ZONE, /* 正在解析Zone相关配置 */
    PARSING_ZONESET, /* 正在解析ZoneSet相关配置 */
    PARSING_TUNNEL, /* 正在解析TUNNEL相关配置 */
    PARSING_INPUTFRANME, /* 正在解析inputframe相关配置 */
    PARSER_ERROR /* 错误 */
};

/*解析器的上下文*/
struct ParserContext {
    enum ParserState state;
    int zone;
    int zoneset;
    int tunnel;
};

/*保存命令解析结果*/
struct Command {
    int type;
    const char *key;
    const char *val;
}


int process_line(char *line, struct ParserContext *ctx, struct Config *cfg)
{
    int ec = 0;
    struct Command cmd;

    /* 这个是你的解析单行的解析函数, 结果会被放进cmd里 */
    parse_command(&cmd, line);

    if (cmd.type == SWITCH_ZONE) {
        /* 对应Zone这条 */
        ctx->state = PARSING_ZONE;
        /* 解析出是哪个zone*/
        ctx->zone = strtol(cmd.val);
    } else if(cmd.type == ADD_MEMBER) {

        if (ctx->state != PARSING_ZONE) {
            /* 如果当前parser状态不是正在设置parsing zone，说明命令错位了，返回错误代码 */
            ctx->state = PARSER_ERROR;
            ec = -1;
            goto cleanup;
        }

        /* 获取Zone结构体 */
        struct Zone *zone = zone_get(cfg, ctx->zone);
        /* 把解析出来的member添加进zone里 */
        zone_member_add(zone, cmd.val);

    } else if (cmd.type == SWITCH_ZONESET) {
        ctx->state = PARSING_ZONESET;
        ctx->zoneset = strtol(cmd.val);
    } else if (cmd.type == ADD_ZONE) {
        if (ctx->state != PARSING_ZONESET) {
            ctx->state = PARSER_ERROR;
            ec = -2;
            goto cleanup;
        }
    } else {
        /* 同理 ... */
    }

cleanup:
    return ec;
}

int parse()
{
    rc = 0, ec = 0;
    struct ParserContext ctx;
    memset(&ctx, 0, sizeof(ParserContext));

    char linebuf[2048];
    while (1) {
        rc = readline(linebuf);
        if (rc == EOF)
            break;

        /* 这里的cfg指的是你保存整个配置数据的数据结构 */
        ec = process_line(linebuf, ctx, cfg);
        if (ec != 0) {
            /* 解析出错了, 退出循环 */
            goto cleanup;
        }
    }

cleanup:
    return ec;
}

