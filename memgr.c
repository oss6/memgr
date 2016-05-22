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
    ofree((void*)(up + 1));
    return freep;
}

void *omalloc(unsigned nbytes)
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

void ofree(void *ap)
{
    union chunk_header *bp, *p;

    bp = (union chunk_header*)ap - 1;
    for (p = freep; !(bp > p && bp < p->s.next); p = p->s.next)
	if (p >= p->s.next && (bp > p || bp < p->s.next))
	    break;

    if (bp + bp->s.size == p->s.next) {
	bp->s.size += p->s.next->s.size;
	bp->s.next = p->s.next->s.next;
    }
    else
	bp->s.next = p->s.next;

    if (p + p->s.size == bp) {
	p->s.size += bp->s.size;
	p->s.next = bp->s.next;
    }
    else
	p->s.next = bp;

    freep = p;
}
