#include "common.h"
#include "util/mem_cache_util.h"
#include "util/lmdb_cache_util.h"
#include "util/stat_util.h"
#include "util/hash_util.h"

void* thread_func(void* userdata)
{
	int retVal = SC_SUCCESS;
    time_t cur_time;

	//local_data_t thread_t;
	//retVal = create_thread_data(&thread_t);	
	local_data_t* thread_t = create_thread_data();
	
	//Storing id, path of thread
	thread_t->tinfo_t->thread_id = ( (wrapper_g_config_t*)userdata)->id;

	strcpy(thread_t->tinfo_t->path, ((wrapper_g_config_t*)userdata)->thread_t->path_names_array[((wrapper_g_config_t*)userdata)->id]);

	if(thread_t->tinfo_t->path == NULL)
	{
		printf("ERROR: path is NULL \n");
		goto cleanup;
    }

	//getting scan start time from predefined function
    time(&cur_time);
    thread_t->info_t->start_time = localtime(&cur_time);
	
	
    printf("Thread Id: %d, Directory_Path: %s \n\n", thread_t->tinfo_t->thread_id, thread_t->tinfo_t->path);
	
    //Collecting all the information about the directory using stat()
    retVal = stat(thread_t->tinfo_t->path,&thread_t->buffer);

    if(retVal != SC_SUCCESS){
		printf("ERROR: Can not perform stat()\n");
        goto cleanup;
    }   

    print_stat(thread_t);    
	
	
    //Scanning the root directory
    retVal = scan_dir(thread_t);

    if(retVal != SC_SUCCESS){
        goto cleanup;
    }

	time(&cur_time);
    thread_t->info_t->end_time = localtime(&cur_time);
	
    //Critical Section Starts (Mutex Needed)
	
	//Mutex lock
	pthread_mutex_lock(&lock);

    //inserting key(inode) value(dir_info) inside hash_table
    retVal = hash_operations(0, ((wrapper_g_config_t*)userdata)->thread_t, thread_t);

	if(retVal != SC_SUCCESS)
	{
		//Unlock Mutex First
		pthread_mutex_unlock(&lock);

		goto cleanup;
	}

    //Mutex Unlock	
	pthread_mutex_unlock(&lock);	

	//Critical Section Ends	
	
   
cleanup:
	//free_local_thread_data(thread_t);	
    return 0;
}

int main(int argc, char** argv)
{
	int retVal = SC_SUCCESS, i=0;

	if(argc != 2)
	{
		printf("Usage: bin/statrun <Provide CACHE_TYPE: MEM_CACHE or LMDB>\n");
		return 1;
	}

	//creating g_config_t object
	g_config_t* thread_t = create_g_config();
	
	if(thread_t == NULL)
	{
		printf("ERROR: can not create g_config_t object\n");
		goto clean;
	}
	
	//creating THREADS_MAX number of threads
	thread_t->thread = (pthread_t*) malloc(THREADS_MAX* sizeof(pthread_t));
	
	if(thread_t->thread == NULL)
	{
		 printf("ERROR: can not create thread object\n");
         goto clean;
	}
	
	//creating hash table to store directory info
    retVal = create_hash(thread_t);

    if(retVal != SC_SUCCESS)
    {
        goto clean;
    }

	//opening the file with all directories path                       
    thread_t->MAX_DIR = read_dir_file(thread_t);
   
	//initializing the mutex object
	retVal = pthread_mutex_init(&lock, NULL);

	if(retVal != SC_SUCCESS)
	{
		goto clean;
	}
                                                                   
	//creating threads for scaning each path individually
	while (i < thread_t->MAX_DIR)
	{
		wrapper_g_config_t *temp_t = (wrapper_g_config_t*) malloc(sizeof(wrapper_g_config_t));
		temp_t->id = i;
		temp_t->thread_t = thread_t;
	
		retVal = pthread_create(&thread_t->thread[i], NULL, thread_func, (void*)(temp_t));
		
		if(retVal != SC_SUCCESS)
		{
			 printf("ERROR: can not create thread with id: %d\n",i);
	         goto clean;
		}	
		i++;
	}
	
	for (i =0 ; i < thread_t->MAX_DIR; i++)
	{
		retVal = pthread_join(thread_t->thread[i], NULL);
		if(retVal != SC_SUCCESS)
		{
			 printf("ERROR: pthread_join error for thread with id: %d\n",i);
             goto clean;
		}
	}

	//iterating the hashtable to display all the directories
    retVal = hash_operations(1,thread_t, NULL);

    if(retVal != SC_SUCCESS)
    {
   		 goto clean;
    }
    
	if (strcmp(argv[1], "MEM_CACHE") == 0)
	{
		//initializing the memcache_st object & servers
		retVal = mem_cache_init();
   		if(retVal != SC_SUCCESS)
    	{
        	goto clean;
    	}	

		//testing mem_cache_query
		retVal = mem_cache_query(thread_t, "12058625");
		if(retVal != SC_SUCCESS)
    	{
       		goto clean;
    	}	
	}
	else
	{
		//initializing the lmdb_cache
		retVal = lmdb_cache_init();
		if(retVal != SC_SUCCESS)
		{
			goto clean;
		}	
	
		//testing lmdb_cache_query
		retVal = lmdb_cache_query(thread_t, "12058625");
	
		if(retVal != SC_SUCCESS)
		{
			goto clean;
		}
	}

clean:
	
	if (strcmp(argv[1], "LMDB") == 0)
		lmdb_close();

	if(thread_t->thread!=NULL)
		free(thread_t->thread);
  	
	thread_t->thread = NULL;
	
	free_hash_table(thread_t);
	
	if(thread_t!=NULL)
		free(thread_t);
		
	thread_t = NULL;
	
	return 0;
}
