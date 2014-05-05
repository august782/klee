#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <klee/klee.h>

int val = 0;

void test1() {
    printf("Test 1\n");
    assert(val == 0);
    val = 1;
}

void test2() {
    printf("Test 2\n");
    assert(val == 1);
    val = 2;
}

void test3() {
    printf("Test 3\n");
    assert(val == 2);
    val = 3;
}

int main(int argc, char **argv) {
    int i;
    void (*foo[3])();
    void (*sym_foo[3])();

    foo[0] = &test1;
    foo[1] = &test2;
    foo[2] = &test3;

    printf("test1: %p\n", &test1);
    printf("test2: %p\n", &test2);
    printf("test3: %p\n", &test3);

    klee_make_symbolic(sym_foo, sizeof(sym_foo), "foo");
    klee_permutate(sym_foo, foo, 3, sizeof(foo[0]));

    for (i = 0; i < 3; ++i) {
        (*sym_foo[i])();
    }
}
