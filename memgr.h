#ifndef MEMGR_H
#define MEMGR_H

#define NALLOC 1024

union chunk_header {
    struct {
	union chunk_header *next;
	unsigned size;
    } s;
    long x;
};

static union chunk_header *more_core(unsigned);
void *omalloc(unsigned);
void ofree(void*);

#endif //MEMGR_H
