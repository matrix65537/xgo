#ifndef __SDS_H__
#define __SDS_H__

#define SDS_MAX_PREALLOC (1024 * 1024)

#include <sys/types.h>
#include <stdarg.h>

typedef char* sds;

struct sdshdr{
    int len;
    int free;
    char buf[];
};

static inline size_t sdslen(const sds s)
{
    struct sdshdr* sh = (void*)(s - (sizeof(struct sdshdr)));
    return sh->len;
}

static inline size_t sdsavail(const sds s)
{
    struct sdshdr* sh = (void*)(s - (sizeof(struct sdshdr)));
    return sh->free;
}

sds sdsnewlen(const void* init, size_t initlen);
sds sdsnew(const void* init);
sds sdsempty(void);
size_t sdslen(const sds s);
void sdsfree(sds s);

#endif

