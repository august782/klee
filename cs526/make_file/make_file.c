#include <klee/klee.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>


void setup() {
    remove("tmp.txt");
}

void teardown() {
    remove("tmp.txt");
}

// Makes a file called 'tmp.txt'
// Caveat: Sometimes does not actually make the file, possibly due to permissions
void make_file(const char *fname) {
    FILE* file;
    file = fopen(fname, "w");
    if (file != NULL) {
        fclose(file);
    }
}

// Checks if file exists
// Caveat: Taken from stack overflow: http://stackoverflow.com/questions/230062/whats-the-best-way-to-check-if-a-file-exists-in-c-cross-platform
int exists(const char *fname) {
    int fd;
    if (fd = open(fname, O_RDONLY)) {
        close(fd);
        return 1;
    }
    return 0;
}

void test_make_file_fail() {
    char fname[8];
    char buf[10];
    ssize_t size;
    FILE* file;
    klee_make_symbolic(fname, sizeof(fname), "fname");
    klee_assume(fname[0] == 't');
    klee_assume(fname[1] == 'm');
    klee_assume(fname[2] == 'p');
    klee_assume(fname[3] == '.');
    klee_assume(fname[4] == 't');
    klee_assume(fname[5] == 'x');
    klee_assume(fname[6] == 't');
    klee_assume(fname[7] == '\0');
    make_file(fname);

    // Assert file tmp.txt exists
    assert(access(fname, R_OK) == 0);
}

int main(int argc, char **argv) {
    setup();
    test_make_file_fail();
    teardown();
}
