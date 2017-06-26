#include "common.h"

int read_dir_file(g_config_t* thread_t)
{
	int j=0;
	fp = fopen("directory_names.txt", "r");

	if(fp==NULL)
	{
		printf("ERROR: FILE OPEN ERROR\n");
		return 1;
	}
	
	char temp_path[PATH_MAX];
	while( fgets(temp_path, sizeof(temp_path), fp))
	{
		thread_t->path_names_array[j] = (char*) malloc (sizeof(char)*PATH_MAX);
		
		char*c = strchr(temp_path, '\n');
		if(c)
			*c = 0;
		
		if(temp_path!=NULL && strlen(temp_path) > 0)
		{
			strcpy(thread_t->path_names_array[j], temp_path);
			j++;
		}
	}
	
	if(fp!=NULL)
		fclose(fp);
	return j;
}



int create_value_for_hash(local_data_t* thread_t)
{
	thread_t->info_t = (value_for_hash_t*) malloc (sizeof(value_for_hash_t));
	
	if(thread_t->info_t==NULL)
	{
		printf("ERROR: malloc() error\n");
		return 1;
	}
	
	thread_t->info_t->no_of_files = 0;
	thread_t->info_t->no_of_dirs = 0;

	thread_t->info_t->start_time = NULL;
	thread_t->info_t->end_time = NULL;

	return 0;
}

void disp_value_for_hash(local_data_t* thread_t)
{
	printf("\nNo of Files: %d \n",thread_t->info_t->no_of_files);
	printf("\nNo of Directories: %d \n",thread_t->info_t->no_of_dirs);

	printf("\nScan Start Time: %d:%d:%d\n",thread_t->info_t->start_time->tm_hour,thread_t->info_t->start_time->tm_min,thread_t->info_t->start_time->tm_sec);

	printf("\nScan Finish Time: %d:%d:%d\n",thread_t->info_t->end_time->tm_hour,thread_t->info_t->end_time->tm_min,thread_t->info_t->end_time->tm_sec);


}


int free_local_thread_data(local_data_t* thread_t)
{
	if(thread_t != NULL)
	{
		if(thread_t->info_t!=NULL)
			free(thread_t->info_t);
		thread_t->info_t = NULL;
		
		if(thread_t->tinfo_t!=NULL)
			free(thread_t->tinfo_t);
		thread_t->tinfo_t = NULL;

		thread_t = NULL;
		
		return 0;
	}
	return 1;
}

local_data_t* create_thread_data()
{
	local_data_t *thread_t = (local_data_t*) malloc(sizeof(local_data_t));
	
	create_value_for_hash(thread_t);
	
	thread_t->tinfo_t = (thread_data_t*) malloc (sizeof(thread_data_t));

	if(thread_t->tinfo_t==NULL)
	{
		printf("ERROR: malloc() error\n");
		return NULL;
	}

	thread_t->tinfo_t->thread_id = 0;			

	thread_t->inode_no = 0;

	return thread_t;
}

g_config_t* create_g_config()
{
	g_config_t* config_t = (g_config_t*) malloc (sizeof(g_config_t));
		
	if (config_t == NULL)
	{
		printf("ERROR: malloc() error\n");
		return NULL;
	}
	
	config_t->directory_hash = NULL;

	config_t->MAX_DIR = 0;
	
	config_t->thread = NULL;
	
	return config_t;
}


