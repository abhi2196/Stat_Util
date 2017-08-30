int main()
{

	int retVal = SC_SUCCESS;
	time_t cur_time;

	//getting scan start time from predefined function
	time(&cur_time);
	start_time = localtime(&cur_time);
	

	//Creates Path for the Root directory to be scanned 
	retVal = scan_path();
	
	if(retVal != SC_SUCCESS){
		goto cleanup;
	}

	printf("%s \n\n",path);

	//Collecting all the information about the directory using stat()
	retVal = stat(path,&buffer);

	if(retVal != SC_SUCCESS){
		goto cleanup;
	}	

	print_stat(&buffer);	

	//Creating Directory info object for extra information
	create_dir_info();
    
   	//Scanning the root directory through check_dir()
	retVal = check_dir(&buffer,path);

	if(retVal != SC_SUCCESS){
		goto cleanup;
	}

	time(&cur_time);
	end_time = localtime(&cur_time);

	//Updating the scan time of the root directory
	update_dir_info();

	//creating hash table to store directory info
	create_hash();

	//inserting key(inode) value(dir_info) inside hash_table
	insert_hash(&buffer,info_t);

	//iterating the hashtable to display all the directories
	iterate_hash();

cleanup:
	//freeing the memory allocated to hash_table
	destroy_hash();

	//freeing dir_info object
	free_dir_info();

	return retVal;
}
