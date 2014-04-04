#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <klee/klee.h>

typedef struct {
    int* values;
    int length;
} values_array;

#define LENGTH 3
int correct_values[LENGTH] = {0,1,2};

unsigned is_correct(int x) {
    int i;
    for (i = 0; i < LENGTH; ++i) {
        if (x == correct_values[i]) {
            return (unsigned)1;
        }
    }
    return (unsigned)0;
}

// Returns a list of file names (string) from current directory
//values_array *get_values() {
//    values_array* array = malloc(sizeof(values_array));
//    array->values = malloc(sizeof(int) * 3);
//    array->values[0] = 0;
//    array->values[1] = 1;
//    array->values[2] = 2;
//    array->length = 3;
//
//    return array;
//}
values_array *get_values() {
    values_array* array = malloc(sizeof(values_array));
    array->values = malloc(sizeof(int) * 3);
    array->length = 3;
    klee_make_symbolic(array->values, sizeof(int) * 3, "values"); 
    //klee_assume(is_correct(array->values[0]));
    //klee_assume(is_correct(array->values[1]));
    //klee_assume(is_correct(array->values[2]));
    klee_assume((array->values[0] == correct_values[0]) | (array->values[0] == correct_values[1]) | (array->values[0] == correct_values[2]));
    klee_assume((array->values[1] == correct_values[0]) | (array->values[1] == correct_values[1]) | (array->values[1] == correct_values[2]));
    klee_assume((array->values[2] == correct_values[0]) | (array->values[2] == correct_values[1]) | (array->values[2] == correct_values[2]));

    return array;
}

void test_get_values_strong() {
    values_array* array = get_values();

    assert(array->length == LENGTH);
    assert(array->values[0] == correct_values[0]);
    assert(array->values[1] == correct_values[1]);
    assert(array->values[2] == correct_values[2]);
}

void test_get_values_weak() {
    values_array* array = get_values();

    assert(array->length == LENGTH);
}

int main() {
    test_get_values_strong();
    test_get_values_weak();
}
