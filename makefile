all: obj/stat_util.o obj/common.o obj/hash_util.o obj/mem_cache_util.o obj/lmdb_cache_util.o bin/statrun
obj/stat_util.o: src/stat_util.c
	gcc -c -I ./include src/stat_util.c -o obj/stat_util.o `pkg-config --cflags glib-2.0`
obj/common.o: src/common.c
	gcc -c -I ./include src/common.c -o obj/common.o `pkg-config --cflags glib-2.0`
obj/hash_util.o: src/hash_util.c
	gcc -c -I ./include src/hash_util.c -o obj/hash_util.o `pkg-config --cflags glib-2.0`
obj/mem_cache_util.o: src/mem_cache_util.c
	gcc -c -I ./include src/mem_cache_util.c -o obj/mem_cache_util.o -l memcached -l pthread `pkg-config --cflags glib-2.0`
obj/lmdb_cache_util.o: src/lmdb_cache_util.c
	gcc -c -I ./include src/lmdb_cache_util.c -o obj/lmdb_cache_util.o `pkg-config --cflags glib-2.0`
bin/statrun: obj/stat_util.o obj/hash_util.o obj/common.o obj/mem_cache_util.o obj/lmdb_cache_util.o src/stat1.c
	gcc -g -I ./include obj/stat_util.o obj/hash_util.o obj/common.o obj/mem_cache_util.o obj/lmdb_cache_util.o src/stat1.c -o bin/statrun -l memcached -l pthread -l lmdb `pkg-config --cflags --libs glib-2.0`
clean:
	 $(RM) obj/stat_util.o obj/common.o obj/hash_util.o obj/mem_cache_util.o obj/lmdb_cache_util.o bin/statrun  
