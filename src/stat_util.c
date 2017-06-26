#include "stat_util.h"

void print_stat(local_data_t* thread_t)
{	
	printf("Information about the file:\n \n");

	ll dev_id=(ll)thread_t->buffer.st_dev;
	printf("ID of device containing file: %lu \n",dev_id);

	ll ino=(ll)thread_t->buffer.st_ino;
	printf("Inode Number: %lu \n",ino);

	ll mode=(ll)thread_t->buffer.st_mode;
	printf("Protection: %lu \n",mode);

	ll nlink=(ll)thread_t->buffer.st_nlink;
	printf("Number of hard links: %lu \n",nlink);
             
	ll uid=(ll)thread_t->buffer.st_uid;
	printf("User ID of owner: %lu \n",uid);
               
    ll gid=(ll)thread_t->buffer.st_gid;
	printf("Group ID of owner: %lu \n",gid);
               
    ll size=(ll)thread_t->buffer.st_size;
	printf("Total size, in bytes: %lu \n",size);            
         
    ll blksize=(ll)thread_t->buffer.st_blksize;
	printf("Blocksize for filesystem I/O:  %lu \n",blksize);                 
    ll blocks=(ll)thread_t->buffer.st_blocks;
	printf("Number of 512B blocks allocated:  %lu \n",blocks); 
}


int scan_dir(local_data_t* thread_t)
{	
	DIR *d;
	struct dirent *dir;
	
	if( ( thread_t->buffer.st_mode & S_IFMT) == S_IFDIR)
	{	
		printf("\nit's a directory\n");
	
		thread_t->inode_no = (ll)thread_t->buffer.st_ino;	
	
		d = opendir(thread_t->tinfo_t->path);

		if(d!=NULL)
		{
			char temp_path[PATH_MAX];

			//Copying path in temp_path to get path for each file.
			strcpy(temp_path,thread_t->tinfo_t->path);

			while( ( dir=readdir(d)) != NULL)
			{
				if( !strcmp(dir->d_name, ".") || !strcmp(dir->d_name,".."))
					continue;

				printf("\nFile Name: %s \n",dir->d_name);

				char name[NAME_MAX];
			
				strcpy(name,"/");			
				strcat(name,dir->d_name);
		
				strcat(thread_t->tinfo_t->path, name);
				printf("\n%s\n",thread_t->tinfo_t->path);

				if(!stat(thread_t->tinfo_t->path, &thread_t->buffer))
				{
					print_stat(thread_t);
				
					//updating the directory & file count inside root directory
					if((thread_t->buffer.st_mode & S_IFMT)==S_IFDIR)
						thread_t->info_t->no_of_dirs+=1;
					else
						thread_t->info_t->no_of_files+=1;
				}
				else
				{
					printf("ERROR: STAT() ERROR\n");
				}
				strcpy(thread_t->tinfo_t->path, temp_path);
			}

			closedir(d);
			d = NULL;
			return 0;
		}	
		else 
		{
			printf("ERROR: CAN NOT OPEN DIRECTORY\n");
			return 1;
		}
	}
		printf("\nit's a file\n");
		return 1;
}


