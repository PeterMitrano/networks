all: student2A.c student2B.c student2_common.c student2_common.h project2.h  project2.c crc32.h crc32.c
	gcc -std=c99 -g -O0 project2.c student2_common.c student2A.c student2B.c crc32.c -o project2

test: student2_common.c student2_common.h project2.h project2.c crc32.h crc32.c queue_test.c
	gcc -std=c99 -g -O0 student2_common.c queue_test.c crc32.c -o queue_test

clean:
	rm project2
