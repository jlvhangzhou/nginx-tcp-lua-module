#ifndef NGX_TCP_LUA_COMMON_H
#define NGX_TCP_LUA_COMMON_H

#include <ngx_config.h>
#include <ngx_core.h>

#include <assert.h>
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#include "ngx_tcp.h"

typedef struct ngx_tcp_lua_main_conf_s {
    lua_State       *lua;

    ngx_str_t        lua_path;
    ngx_str_t        lua_cpath;

    ngx_pool_t      *pool;
} ngx_tcp_lua_main_conf_t;


typedef struct ngx_tcp_lua_srv_conf_s {
    ngx_str_t                 lua_file;
    size_t                    buffer_size;

    ngx_msec_t                       read_timeout;
    ngx_msec_t                       send_timeout;
    ngx_msec_t                       connect_timeout;
} ngx_tcp_lua_srv_conf_t;


typedef void (*ngx_tcp_cleanup_pt)(void *data);


typedef struct ngx_tcp_lua_ctx_s {
    void                    *data;
    
    lua_State               *co;
    
    int              cc_ref;            /*  reference to anchor coroutine in
                                            the lua registry */

    int              ctx_ref;           /*  reference to anchor request ctx
                                            data in lua registry */

    ngx_chain_t             *free_recv_bufs;


    ngx_tcp_cleanup_pt     *cleanup;

    unsigned         socket_busy:1;
    unsigned         socket_ready:1;
} ngx_tcp_lua_ctx_t;

#define DDEBUG 1

#if defined(DDEBUG) && (DDEBUG)

#   if (NGX_HAVE_VARIADIC_MACROS)

#       define dd(...) fprintf(stderr, "redis2 *** "); \
            fprintf(stderr, __VA_ARGS__); \
            fprintf(stderr, " at %s line %d.\n", __FILE__, __LINE__)

#   else

#include <stdarg.h>
#include <stdio.h>

#include <stdarg.h>

static void dd(const char* fmt, ...) {
}

#    endif

#else

#   if (NGX_HAVE_VARIADIC_MACROS)

#       define dd(...)

#   else

#include <stdarg.h>

static void dd(const char* fmt, ...) {
}

#   endif

#endif

/*  coroutine anchoring table key in Lua vm registry */
#define NGX_LUA_CORT_REF "ngx_lua_cort_ref"

/*  request ctx data anchoring table key in Lua vm registry */
#define NGX_LUA_REQ_CTX_REF "ngx_lua_req_ctx_ref"


/*  globals symbol to hold nginx request pointer */
#define GLOBALS_SYMBOL_REQUEST    "ngx._req"

/*  globals symbol to hold code chunk handling nginx request */
#define GLOBALS_SYMBOL_RUNCODE    "ngx._code"


extern ngx_module_t ngx_tcp_lua_module;


#endif