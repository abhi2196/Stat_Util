#ifndef __COMMON_H_
#define __COMMON_H_

#include <stdlib.h>
#include <sys/stat.h>
#include <glib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <pthread.h>

#define PATH_MAX 4096
#define NAME_MAX 255
#define DIRS_MAX 5
#define THREADS_MAX 5
#define TEST_DIR_PATH "test/directory_names.txt"
#define ll unsigned long int

enum SC_ERRORCODES{
	SC_SUCCESS = 0,
	SC_FAILURE = 1
};

enum HASH_OPCODES{
	HASH_INSERT = 0,
	HASH_ITERATE = 1,
};

FILE* fp;
size_t path_len;
pthread_mutex_t lock;

//data-type for entry value of each directory in hash_table
typedef struct value_for_hash
{
	int no_of_dirs;
	int no_of_files;
	
	struct tm *start_time;
	struct tm *end_time;

}value_for_hash_t;

//global_data like hash_table, MAX_DIR etc
typedef struct g_config
{

	GHashTable *directory_hash;

	pthread_t *thread;
	
	char *path_names_array[DIRS_MAX];

	int MAX_DIR;
	
}g_config_t;

//thread's id, path info
typedef struct thread_data
{
	int thread_id;
	char path[PATH_MAX];
}thread_data_t;

//local_data for each thread
typedef struct local_data
{
	struct stat buffer;
	long int inode_no;
	value_for_hash_t *info_t;
	thread_data_t *tinfo_t;
}local_data_t;

//wrapper for thread_local func
typedef struct wrapper_g_config
{
    int id;
    g_config_t *thread_t;
}wrapper_g_config_t;

//g_config  operations
g_config_t* create_g_config();

//read all the directory names from file
int read_dir_file(g_config_t*);

//value_for_hash operations
int create_value_for_hash(local_data_t*);
void disp_value_for_hash(local_data_t*);
void calculate_scan_time(local_data_t*);

//thread_data operation
local_data_t* create_thread_data();
int free_local_thread_data(local_data_t*);

#endif /*__COMMON_H_*/

