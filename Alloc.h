#ifndef ALLOC_H
#define ALLOC_H

#include <cstddef>
#include <cstdlib>
#include <cstring>   //memcpy
#include <iostream>
#include "Config.h"

namespace DLib
{

using std::cerr;
using std::endl;
using std::cout;

class mallocAlloc
{
private:
    static void* malloc_doerr(size_t num);

    static void* realloc_doerr(void* addr, size_t newsz);

    typedef void (*mallocErrHandler)();

    static mallocErrHandler handler;

public:
    static void* allocate(size_t n);

    static void deallocate(void* p);

    static void* reallocate(void* p, size_t newsz);

    static mallocErrHandler setMallocErrHandler(mallocErrHandler newhandler)
    {
        mallocErrHandler old = handler;
        handler = newhandler;
        return old;
    }
};

class MemoryPoolAlloc
{
private:
    enum {ALIGN=8};
    enum {BYTESMAX=128};
    enum {FREELISTNUM=16};

    union Block
    {
        Block* next;
        char cliData[1];
    };

    static size_t raiseUp(size_t bytes)
    {
        return (bytes+ALIGN-1) & ~(ALIGN-1);
    }

private:
    static char*  startPool;
    static char*  endPool;
    static size_t poolSize;

    static Block* freeBlockList[FREELISTNUM];

    static size_t findPropelIndex(size_t bytes)
    {
        return bytes / ALIGN;
    }

    static void* refill(size_t size);

    static char* chunkAlloc(size_t size, int& nblocks);

public:
    static void* allocate(size_t n);

    static void* reallocate(void* addr, size_t odsz, size_t newsz);

    static void  deallocate(void* addr, size_t n);
};

template<class T, class Alloc>
class standardAlloc
{
public:
    static T* allocate(size_t n)
    {
        return (n==0) ? NULL : (T*)Alloc::allocate(n*sizeof(T));
    }
    static T* allocate(void)
    {
        return (T*)Alloc::allocate(sizeof(T));
    }
    static void dealloate(T* p, size_t n)
    {
        if(n!=0)
            Alloc::deallocate(p, n*sizeof(T));
    }
    static void deallocate(T* p)
    {
        Alloc::deallocate(p, sizeof(T));
    }
};



typedef MemoryPoolAlloc poolAlloc;


} /*namespace DLib*/

#endif // ALLOC_H
