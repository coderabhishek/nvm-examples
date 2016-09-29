#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libpmemobj.h>

#define filename "mydata.out"
#define LAYOUT_NAME "buff_store"
#define MAX_NUM_OF_WRITES 20

typedef struct {
  int id;
  char name[100];
} person;

struct root{
	int size;
	person data[MAX_NUM_OF_WRITES];
};

typedef struct {
    int dummy;
} dummy_struct;


void print_existing_records() {
    /* Read and interpret file from storage. */
    PMEMobjpool *pool = pmemobj_open(filename, LAYOUT_NAME);
    if (pool == NULL) {
		fprintf(stderr, "Error opening persistent pool while reading!!");
		return;
	}
	PMEMoid root = pmemobj_root(pool, sizeof(struct root));
	struct root *p = pmemobj_direct(root);
	int num_of_entries = p->size;
	int i;
	for(i=0;i<num_of_entries;++i)
		printf("%d  %s\n", p->data[i].id, p->data[i].name);

    pmemobj_close(pool);
}

int main(int argc, char* argv[]) {
    if (argc != 2 && argc != 4) {
	printf("Usage: prog read OR prog write id name\n");
	exit(1);
    }
  
    if (!strcmp(argv[1], "read")) {
	print_existing_records();
    }
    else if (!strcmp(argv[1], "write")) {
	assert(argc == 4);
	/* Write to storage. */

	PMEMobjpool *pool = pmemobj_create(filename, LAYOUT_NAME, PMEMOBJ_MIN_POOL, 0666);

	//if the pool is already created then just try to open it, if open also fails then print error
	if(pool == NULL) {
	    pool = pmemobj_open(filename, LAYOUT_NAME);
	    if(pool == NULL){
			fprintf(stderr, "Error allocating resource on NVM!!\n");
			return 1;
		}
	}

	PMEMoid root = pmemobj_root(pool, sizeof (struct root));
	struct root *p = pmemobj_direct(root);
	p->data[p->size].id = atoi(argv[2]);
	pmemobj_persist(pool, &(p->data[p->size].id), sizeof(int));
	pmemobj_memcpy_persist(pool, p->data[p->size].name, argv[3], 100);
	p->size += 1;
	pmemobj_persist(pool, &p->size, sizeof(int));
	pmemobj_close(pool);

    } else {
	printf("Unknown argument.\n");
	exit(1);
    }
    return 0;
}
