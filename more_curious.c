/*
 * clang -Wall -fblocks -framework Foundation more_curious.c -o more_curious
 */
#include <stdio.h>
#include <Block.h>

struct Block_byref {
    void *isa;
    struct Block_byref *forwarding;
    int flags; /* refcount; */
    int size;
    void (*byref_keep)(struct Block_byref *dst, struct Block_byref *src);
    void (*byref_destroy)(struct Block_byref *); 
    /* long shared[0]; */
};

static __inline struct Block_byref* derefBlockVar(char* src) 
{
    return (struct Block_byref*) (src - 2*sizeof(int) - 2*sizeof(void *));
}

extern const char *_Block_dump(const void *block);
extern const char *_Block_byref_dump(struct Block_byref *src);



typedef void(^BoringBlock)(void);
void (^boringBlock)(void);

BoringBlock blockRefCountTest(void)
{
    __block int x = 1;

    printf("Before local block:\n%s\n\n",_Block_byref_dump(derefBlockVar((char*)&x)));
    BoringBlock localBlock = ^{
        x++;
        printf("Execute block:\n%s\n",_Block_byref_dump(derefBlockVar((char*)&x)));
        printf("x is %d, &x is %p\n", x, &x);
    };
    printf("After local block generated:\n%s\n\n",_Block_byref_dump(derefBlockVar((char*)&x)));

    boringBlock = Block_copy(localBlock);
    printf("After first block copy:\n%s\n\n",_Block_byref_dump(derefBlockVar((char*)&x)));

    BoringBlock retBlock = Block_copy(localBlock);
    printf("After second block copy:\n%s\n\n",_Block_byref_dump(derefBlockVar((char*)&x)));
    return retBlock;
}

int main (void)
{
    BoringBlock retBlock = blockRefCountTest();
    boringBlock();
    Block_release(boringBlock);
    retBlock();
    Block_release(retBlock);

    return 0;
}

