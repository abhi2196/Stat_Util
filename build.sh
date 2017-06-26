set -x
rm -f obj/stat_util.o
rm -f bin/statrun
gcc -c -I ./include src/stat_util.c -o obj/stat_util.o
gcc -I ./include obj/stat_util.o src/stat1.c -o bin/statrun

