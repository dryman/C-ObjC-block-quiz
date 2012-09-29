/*
 * clang -Wall -fblocks -framework Foundation quiz.c -o quiz
 */
#include <stdio.h>
#include <Block.h>

typedef void (^BoringBlock)(void);
void (^boringBlock)(void);

void quiz_1 (void)
{
    __block int x = 1;
    printf("x address is %p\n", &x);

    BoringBlock localBlock = ^{
        x++; // Dummy use of x
        printf("End of quiz 1\n\n");
    };
    boringBlock = Block_copy(localBlock);

    printf("after copy, x address is %p\n", &x);
}

void quiz_2 (void)
{
    __block int x = 1;

    BoringBlock localBlock = ^{
        printf("x is %d\n", x);
        printf("End of quiz 2\n\n");
    };
    boringBlock = Block_copy(localBlock);

    x++;
}

void quiz_3 (void)
{
    __block int x = 1;
    __block int* ptr = &x;

    BoringBlock localBlock = ^{
        printf("x is %d, *ptr is %d\n", x, *ptr);
        printf("End of quiz 3\n\n");
    };
    boringBlock = Block_copy(localBlock);

    x++;
}
void quiz_3_helper (void)
{
    int x = 0;
    printf("clean up stack with x=%d\n",x);
}

/*
void quiz_4(void)
{
    __block int x[1] = {1};

    void (^localBlock)(void) = ^{
        printf("x[0] is %d\n", x[0]);
        printf("End of quiz 4\n\n");
    }
    x[0] = 2;
}
*/

BoringBlock quiz_5(void)
{
    __block int x = 1;

    printf("x address is %p\n", &x);

    BoringBlock localBlock = ^{
        x++;
        printf("x is %d, &x is %p\n", x, &x);
    };
    boringBlock = Block_copy(localBlock);
    printf("x address is %p\n", &x);

    BoringBlock retBlock = Block_copy(localBlock);
    printf("x address is %p\n", &x);

    return retBlock;
}


int main (void)
{
    quiz_1();
    boringBlock();
    Block_release(boringBlock);

    quiz_2();
    boringBlock();
    Block_release(boringBlock);

    quiz_3();
    boringBlock();
    Block_release(boringBlock);

    quiz_3();
    quiz_3_helper();
    boringBlock();
    Block_release(boringBlock);

    BoringBlock retBlock = quiz_5();
    boringBlock();
    retBlock();
    Block_release(boringBlock);
    Block_release(retBlock);
    printf("End of quiz 5\n\n");

    return 0;
}

