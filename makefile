ROOT=.

SRC=${ROOT}/src
OBJ=${ROOT}/obj
BIN=${ROOT}/bin
INC=${ROOT}/include

FLAGS= -g -I
INCLUDES=${INC} 
LIBS= -l memcached -l pthread -l lmdb `pkg-config --cflags --libs glib-2.0`

all:: clean build

build:
	gcc -c ${FLAGS} ${INCLUDES} ${SRC}/util/stat_util.c -o ${OBJ}/util/stat_util.o ${LIBS}
	gcc -c ${FLAGS} ${INCLUDES} ${SRC}/common.c -o ${OBJ}/common.o ${LIBS}
	gcc -c ${FLAGS} ${INCLUDES} ${SRC}/util/hash_util.c -o ${OBJ}/util/hash_util.o ${LIBS}
	gcc -c ${FLAGS} ${INCLUDES} ${SRC}/util/mem_cache_util.c -o ${OBJ}/util/mem_cache_util.o ${LIBS}
	gcc -c ${FLAGS} ${INCLUDES} ${SRC}/util/lmdb_cache_util.c -o ${OBJ}/util/lmdb_cache_util.o ${LIBS}
	gcc ${FLAGS} ${INCLUDES} ${OBJ}/util/stat_util.o ${OBJ}/util/hash_util.o ${OBJ}/common.o ${OBJ}/util/mem_cache_util.o ${OBJ}/util/lmdb_cache_util.o ${SRC}/stat1.c -o ${BIN}/statrun ${LIBS}

clean:
	 rm -f ${OBJ}/*.o
	 rm -f ${OBJ}/util/*.o
	 rm -f ${BIN}/statrun
