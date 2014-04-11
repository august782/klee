#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <klee/klee.h>

typedef struct {
    int* set_values;    // Represents non-deterministic ordering, what user will actually get
    int* values;        // Actual values
    int length;
} set;

set* makeSet() {
    set *s = malloc(sizeof(set));
    s->set_values = malloc(sizeof(int) * 1);
    s->values = malloc(sizeof(int) * 1);
    s->length = 0;

    return s;
}

void add(set* s, int x) {
    // Allocate new space for bigger data
    int* new_set_values = malloc(sizeof(int) * (s->length+1));
    int* new_values = malloc(sizeof(int) * (s->length+1));

    // Copy old (real) values over
    memcpy(new_values, s->values, sizeof(int) * (s->length));
    new_values[s->length] = x;
    s->length++;

    // Free the old data
    free(s->set_values);
    free(s->values);

    // Copy over new pointers
    s->set_values = new_set_values;
    s->values = new_values;
}

int get(set* s, int i) {
    // If already symbolic (non-deterministic ordering), go ahead and return index
    if (klee_is_symbolic(s->set_values[i])) {
        return s->set_values[i];
    }
    // Otherwise make the set symbolic and permutate
    else {
        klee_make_symbolic(s->set_values, sizeof(int) * s->length, "set");
        klee_permutate(s->set_values, s->values, s->length, sizeof(int));
        return s->set_values[i];
    }
}

char* toString(set *s) {
    int i;
    char *str = malloc(sizeof(char) * s->length * 4);
    char *index = str;

    for (i = 0; i < s->length; ++i) {
        sprintf(index, "%d ", get(s, i));
        index = str + strlen(str);
    }

    return str;
}

void testSetToString() {
    set *s = makeSet();
    add(s, 3);
    add(s, 2);
    add(s, 5);
    add(s, 0);
    printf("%d\n", s->values[0]);
    printf("%d\n", s->values[1]);
    printf("%d\n", s->values[2]);
    printf("%d\n", s->values[3]);

    assert(strcmp(toString(s), "3 2 5 0 ") == 0);
}

int main(int argc, char **argv) {
    testSetToString();
}
