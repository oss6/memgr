#ifndef MEMGR_H
#define MEMGR_H

#define NALLOC 1024;

union chunk_header {
    struct {
	union header *next;
	unsigned size;
    } s;
    long x;
};

union chunk_header *more_core(unsigned);
void *malloc(unsigned);
void free(void*);

#endif MEMGR_H
