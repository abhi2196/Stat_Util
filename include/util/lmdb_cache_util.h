#ifndef __LMDB_CACHE_UTIL_H_
#define __LMDB_CACHE_UTIL_H_

#include <stdio.h>
#include <sys/stat.h>
#include "lmdb.h"
#include "common.h"
#include "util/hash_util.h"

#define OPENDBMODE 0777
#define LMDB_DIR_PATH "/lmdb-data"

MDB_env *g_ptrLMDBEnv;
value_for_hash_t* retrieved_value_lmdb;

int lmdb_cache_init();
int lmdb_close();
int lmdb_cache_query(g_config_t*, char*);
int mdb_data_get(char*, value_for_hash_t*);
int mdb_data_put(char*, int, void*, size_t);

#endif /*__LMDB_CACHE_UTIL_H_*/
