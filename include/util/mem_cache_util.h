#ifndef __MEM_CACHE_UTIL_H_
#define __MEM_CACHE_UTIL_H_

#include <libmemcached/memcached.h>
#include <stdio.h>
#include <string.h>
#include "common.h"
#include "util/hash_util.h"

memcached_server_st *servers;
memcached_st *memc;
memcached_return memc_rc;

size_t value_length;
uint32_t flags;

char* retrieved_value;

int mem_cache_init();
int mem_cache_query(g_config_t*, char*);
int mem_cache_put(char*, char*);
int mem_cache_get(char*);

#endif /*__MEM_CACHE_UTIL_H_*/
