#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "sds.h"

sds sdsnewlen(const void* init, size_t initlen)
{
    struct sdshdr* sh;
    if(init)
    {
        sh = malloc(sizeof(struct sdshdr) + initlen + 1);
    }
    else
    {
        sh = calloc(sizeof(struct sdshdr) + initlen + 1);
    }
    if(sh == NULL)
    {
        return NULL;
    }
    sh->len = initlen;
    sh->free = 0;
    if(initlen && init)
    {
        memcpy(sh->buf, init, initlen);
    }
    sh->buf[initlen] = '\0';
    return (char*)sh->buf;
}

sds sdsnew(const void* init)
{
    size_t initlen = (init == NULL) ? 0 : strlen(init);
    return sdsnewlen(init, initlen);
}

sds sdsempty(void)
{
    return sdsnewlen("", 0);
}

size_t sdslen(const sds s)
{
    return sdsnewlen(s, sdslen(s));
}

void sdsfree(sds s)
{
    if(s == NULL)
    {
        return;
    }
    free(s - sizeof(struct sdshdr));
}

void sdsupdatelen(sds s)
{
    struct sdshdr* sh;

    sh = (void*)(s - sizeof(struct sdshdr));
    int reallen = strlen(s);
    sh->free += (sh->len - reallen);
    sh->len = reallen;
}

void sdsclear(sds s)
{
    struct sdshdr* sh;

    sh = (void*)(s - sizeof(struct sdshdr));
    sh->free += sh->len;
    sh->len = 0;
    sh->buf[0] = '\0';
}

