#include <stdio.h>
#include <sys/stat.h>
#include "lmdb.h"
#include "common.h"

#define OPENDBMODE 0777

MDB_env *g_ptrLMDBEnv;
char lmdbDirPath[] = "/lmdb-data";
value_for_hash_t* retrieved_value;
int rc = 0;

int lmdb_cache_init();
int lmdb_close();
int lmdb_cache_query(g_config_t*, char*);
int mdb_data_get(char*, value_for_hash_t*);
int mdb_data_put(char*, int, void*, size_t);
gpointer lookup_hash_table_with_key(g_config_t*, char*);
