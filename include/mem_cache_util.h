#include <libmemcached/memcached.h>
#include <stdio.h>
#include <string.h>
#include "common.h"

memcached_server_st *servers = NULL;
memcached_st *memc;
memcached_return rc;

size_t value_length;
uint32_t flags;

char* retrieved_value;

int mem_cache_init();
int mem_cache_query(g_config_t*, char*);

gpointer lookup_hash_table_with_key(g_config_t*, char*);
