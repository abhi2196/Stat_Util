#include "util/mem_cache_util.h"

int mem_cache_init()
{
  //creating a memcached_st object
  memc = memcached_create(NULL);
  
  //appending the list of memcached servers
  servers = memcached_server_list_append(servers, "localhost", 21201, &memc_rc);

  //pushing the servers
  memc_rc = memcached_server_push(memc, servers);

  if (memc_rc == MEMCACHED_SUCCESS)
    fprintf(stderr, "Added server successfully\n");
  else
  {
    fprintf(stderr, "Couldn't add server: %s\n", memcached_strerror(memc, memc_rc));
	return 1;
  }
   return 0;
}

int mem_cache_query(g_config_t* thread_t, char* key)
{
	//retrieving value from memcache for given key
 	retrieved_value = memcached_get(memc, key, strlen(key), &value_length, &flags, &memc_rc);
    
    if (memc_rc == MEMCACHED_SUCCESS) {
     
	  fprintf(stdout, "\n**************************\n"); 
	  fprintf(stderr, "Key retrieved successfully\n");
	  printf("no. of files: %d\n", ((value_for_hash_t*)retrieved_value)->no_of_files);  
	  printf("no. of dirs: %d\n", ((value_for_hash_t*)retrieved_value)->no_of_dirs);
	  //printf("Start time: %d\n", ((value_for_hash_t*)retrieved_value)->start_time->tm_hour);
    }
    else
	{
      fprintf(stderr, "Couldn't retrieve key: %s\n", memcached_strerror(memc, memc_rc));
	  
	  //retrieve value from database server or hashtable and set it in the memcache
	  
	  gpointer value = lookup_hash_table_with_key(thread_t, key);
	  
      if (value == NULL)
	  {
			printf("Error: value is NULL\n");
			return 1;
	  }
	
  	  memc_rc = memcached_set(memc, key, strlen(key), value, sizeof(value_for_hash_t), (time_t)0, (uint32_t)0);
 
 	  if (memc_rc == MEMCACHED_SUCCESS)
    	  fprintf(stderr, "Key stored successfully\n");
 	  else
 	    fprintf(stderr, "Couldn't store key: %s\n", memcached_strerror(memc, memc_rc));
 	  
	}

	return 0;
}

int mem_cache_put(char* key, char* value)
{
	if (value == NULL)
	{
		printf("Error: value is NULL\n");
		return 1;
	}

	memc_rc = memcached_set(memc, key, strlen(key), value, strlen(value), (time_t)0, (uint32_t)0);

	if (memc_rc == MEMCACHED_SUCCESS)
	{
		fprintf(stderr, "Key stored successfully\n");
		return 0;
	}	
	else
	{
		fprintf(stderr, "Couldn't store key: %s\n", memcached_strerror(memc, memc_rc));
		return 1;
	}
}

int mem_cache_get(char* key)
{
	retrieved_value = memcached_get(memc, key, strlen(key), &value_length, &flags, &memc_rc);
	if (memc_rc == MEMCACHED_SUCCESS){
		printf("Key: %s found !\n", key);
		printf("Value: %s\n", retrieved_value);
		printf("**********************\n");
		return 0;
	}	
	else {
		printf("Key: %s not found !", key);
		return 1;
	}
}
