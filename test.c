#include <stdio.h>
#include <memgr.h>

int main(void)
{
    int n = 10;
    int *t = omalloc(sizeof(int));
    t = &n;

    printf("%d", *t);

    ofree(t);
    
    return 0;
}
