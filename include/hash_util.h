#include "common.h"

int create_hash(g_config_t*);

int hash_operations(int, g_config_t*, local_data_t*);

int free_hash_table(g_config_t*);

gpointer lookup_hash_table_with_key(g_config_t*, gpointer);
