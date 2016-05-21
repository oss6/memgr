#include <unistd.h>
#include "memgr.h"

static union chunk_header base;
static union chunk_header *freep = NULL;

static union chunk_header *more_core(unsigned nu)
{
    char *cp;
    union chunk_header *up;

    if (nu < NALLOC)
	nu = NALLOC;

    cp = sbrk(nu * sizeof(union chunk_header));
    if (cp == (char*) -1)
	return NULL;

    up = (union chunk_header*) cp;
    up->s.size = nu;
    free((void*)(up + 1));
    return freep;
}

void *malloc(unsigned nbytes)
{
    union chunk_header *p, *prevp;
    unsigned nunits = (nbytes + sizeof(union chunk_header) - 1) / sizeof(union chunk_header) + 1;

    if (!(prevp = freep)) {
	base.s.next = freep = prevp = &base;
	base.s.size = 0;
    }

    for (p = prevp->s.next; ; prevp = p, p = p->s.next) {
	if (p->s.size >= nunits) {
	    if (p->s.size == nunits)
		prevp->s.next = p->s.next;
	    else {
		p->s.size -= nunits;
		p += p->s.size;
		p->s.size = nunits;
	    }
	    freep = prevp;
	    return (void*)(p + 1);
	}

	if (p == freep)
	    if (!(p = more_core(nunits)))
		return NULL;
    }
}

void free(void *ap)
{
    
}
