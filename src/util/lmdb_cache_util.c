#include "util/lmdb_cache_util.h"

int mdb_data_put(char *skey, int keySize, void *sval, size_t svalSize){
	
	MDB_txn *txn = NULL;
	MDB_dbi dbi;
	MDB_val key, data;
	int rc = 0;

	//begin lmdb transcation
	rc = mdb_txn_begin(g_ptrLMDBEnv, NULL, 0, &txn);
	
	//open txn
	rc = mdb_open(txn, NULL, 0, &dbi);

	//construct key, val
	key.mv_size = keySize;
	key.mv_data = skey;
	data.mv_size = svalSize;
	data.mv_data = sval;

	printf("svalSize=%ld\n", svalSize);
	
	rc = mdb_put(txn, dbi, &key, &data, 0);
	
	if (rc) {
		printf("mdb_put: %s\n", mdb_strerror(rc));
	}

	rc = mdb_txn_commit(txn);

	if (rc) {
		printf("mdb_txn_commit: (%d) %s\n", rc, mdb_strerror(rc));
	}
	
	mdb_close(g_ptrLMDBEnv, dbi);
	
	return rc;
}

int mdb_data_get(char* key_s, value_for_hash_t* value){

	MDB_txn *txn = NULL;
	MDB_dbi dbi;
	MDB_cursor *cursor;
	MDB_val key, data;
	int flag = 0, key_int = atoi(key_s), rc = 0;

	//begin lmdb transaction
	rc = mdb_txn_begin(g_ptrLMDBEnv, NULL, MDB_RDONLY, &txn);

	//open txn
	rc = mdb_open(txn, NULL, 0, &dbi);

	//open cursor
	rc = mdb_cursor_open(txn, dbi, &cursor);

	//iterate data
	while ((rc = mdb_cursor_get(cursor, &key, &data, MDB_NEXT)) == 0) {

		char *strKey = (char *)key.mv_data;

		if(atoi(strKey) == key_int){
			value->no_of_files = (( value_for_hash_t *) data.mv_data)->no_of_files;
			value->no_of_dirs = (( value_for_hash_t *) data.mv_data)->no_of_dirs;
			flag = 1;
			break;
		}
	}
	
	//cleanup
	mdb_cursor_close(cursor);
	mdb_txn_abort(txn);
	mdb_close(g_ptrLMDBEnv, dbi);
	
	if(flag == 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int lmdb_cache_init()
{
	int rc = 0;

	rc = mdb_env_create(&g_ptrLMDBEnv);
	if(rc != 0){
		fprintf(stderr, "failed to create LMDB environment handle\n");
		return rc;	
	}

	//open env handle
	rc = mdb_env_open(g_ptrLMDBEnv, LMDB_DIR_PATH, 0, OPENDBMODE);
	if(rc != 0){
		fprintf(stderr, "failed to open lmdb\n");
		return rc;	
	}
	retrieved_value_lmdb = (value_for_hash_t*)malloc(sizeof(value_for_hash_t));

	return rc;
}

int lmdb_close(){

	mdb_env_close(g_ptrLMDBEnv);
	if (retrieved_value_lmdb != NULL)
	{
		free(retrieved_value_lmdb);
		retrieved_value_lmdb = NULL;
	}
}


int lmdb_cache_query(g_config_t* thread_t, char* key)
{
	int rc = 0;
    //retrieving value from lmdb for given key
    rc = mdb_data_get(key, retrieved_value_lmdb);
    
    if (rc == 0) 
	{
		fprintf(stdout, "\n**************************\n");
    	fprintf(stderr, "Key retrieved successfully\n");
	  	if(retrieved_value_lmdb == NULL)
	  	{	
			printf("Error: NULL\n");
			return 1;
      	}
	  	else
	  	{
	  		printf("No. of files: %d\n", retrieved_value_lmdb->no_of_files);  
      		printf("No. of dirs: %d\n", retrieved_value_lmdb->no_of_dirs);
	  	}
    }   
    else
    {   

      fprintf(stderr, "Couldn't retrieve key: %s\n", key);
    
      //retrieve value from database server or hashtable and set it in the lmdb
    
      gpointer value = lookup_hash_table_with_key(thread_t, key);
    
      if (value == NULL)
      {   
            printf("Error: value is NULL\n");
            return 1;
      }   
    
      rc = mdb_data_put(key, strlen(key), value, sizeof(value_for_hash_t));
 
      if (rc == 0)
	  {
      	fprintf(stderr, "Key stored successfully\n");
      }
	  else
	  {
      	fprintf(stderr, "Couldn't store key: %s\n", mdb_strerror(rc));
	  }

    }

    return 0;
}

