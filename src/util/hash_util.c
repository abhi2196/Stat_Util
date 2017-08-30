#include "util/hash_util.h"


void iterator(gpointer key, gpointer value, gpointer table_data)
{
	// printf statement for hash_table iterator
	printf(table_data,*(gint*)key, ((value_for_hash_t*)value)->no_of_files, ((value_for_hash_t*)value)->no_of_dirs, ((value_for_hash_t*)value)->start_time->tm_hour, ((value_for_hash_t*)value)->start_time->tm_min, ((value_for_hash_t*)value)->start_time->tm_sec, ((value_for_hash_t*)value)->end_time->tm_hour, ((value_for_hash_t*)value)->end_time->tm_min, ((value_for_hash_t*)value)->end_time->tm_sec, ((value_for_hash_t*)value)->end_time->tm_hour - ((value_for_hash_t*)value)->start_time->tm_hour , ((value_for_hash_t*)value)->end_time->tm_min - ((value_for_hash_t*)value)->start_time->tm_min, ((value_for_hash_t*)value)->end_time->tm_sec - ((value_for_hash_t*)value)->start_time->tm_sec);
}

void free_data(gpointer data)
{
	//freeing the memory allocated dynamically to the values in hash_table
	if(data!=NULL)
		free(data);
	data = NULL;
}

int create_hash(g_config_t* info_t)
{
	info_t->directory_hash = g_hash_table_new_full(NULL, NULL, free_data, free_data);
	
	if (info_t->directory_hash == NULL)
		return 1;

	return 0;
}

int hash_operations(int op_id, g_config_t* g_thread, local_data_t* thread_t)
{
	if (op_id == HASH_INSERT)
	{
		gint* key = g_new(gint, 1);
			
		//storing inode no of directory as key value
		*key = (gint)thread_t->inode_no;
	
		gpointer value = thread_t->info_t;
		
		//returns 1 if key does not exist previously
		if (g_thread->directory_hash !=NULL)
		{
			g_hash_table_insert(g_thread->directory_hash,key,value);
			return 0;
		}
		return 1;
	}
	else if (op_id == HASH_ITERATE)
	{
		g_hash_table_foreach(g_thread->directory_hash,(GHFunc)iterator,"\n\nInode No: %d \n\n 1) No. Of Files: %d\n 2) No. of Directories: %d\n 3) Scan Start time: %d:%d:%d\n 4) Scan Finish time: %d:%d:%d\n 5) Total Scan time: %d:%d:%d\n");
		return 0;	
	}
	else
	{
		return 1;
	}	

}
int free_hash_table(g_config_t* g_thread)
{
	
		if(g_thread == NULL)
			return 1;	
		//removes all key, value pair along with dynamically allocated memory associated with them
		if(g_thread->directory_hash!=NULL)
			g_hash_table_remove_all(g_thread->directory_hash);

		int i;
	
		for (i =0 ;i < g_thread->MAX_DIR; i++)
		{
			if(g_thread->path_names_array[i]!=NULL){
				free(g_thread->path_names_array[i]);
				g_thread->path_names_array[i] = NULL;
			}
		}
		return 0;
}

gpointer lookup_hash_table_with_key(g_config_t* thread_t, gpointer key_t)
{
	GHashTableIter iter;
	gpointer key,value;

	g_hash_table_iter_init(&iter, thread_t->directory_hash);
	while (g_hash_table_iter_next (&iter, &key, &value))
	{
		if(*(gint*)key == atoi(key_t))
		return g_hash_table_lookup(thread_t->directory_hash, key);
	}
}

