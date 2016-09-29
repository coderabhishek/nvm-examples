#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define filename "mydata.out"

typedef struct {
  int id;
  char name[100];
} person;

typedef struct {
    int dummy;
} dummy_struct;

void print_existing_records() {
    /* Read and interpret file from storage. */
    FILE *fp = fopen(filename, "r+");
    char* buf = (char*) malloc(sizeof(person));
    person* p;
    /* Read a person off storage and print it. */
    while (fread(buf, 1, sizeof(person), fp) == sizeof(person)) {
	p = (person*) buf;
	printf("%d %s\n", p->id, p->name);
    }
    free(buf);
    fclose(fp);
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
	person p;
	p.id = atoi(argv[2]);
	strcpy(p.name, argv[3]);
	char* buf = (char*) malloc(sizeof(person));
	memcpy(buf, &p, sizeof(p));
	FILE *fp = fopen(filename, "a");
	fwrite(buf, sizeof(p), 1, fp);
	free(buf);
	fclose(fp);
    } else {
	printf("Unknown argument.\n");
	exit(1);
    }
    return 0;
}
