#include <klee/klee.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

int val=0;

void *func(void* threadid){
  unsigned seconds;
  klee_make_symbolic(seconds, sizeof(int), "seconds");
  sleep(seconds);
  val=5;
  pthread_exit(NULL);
}

void test_func(){
  pthread_t thread;
  int id=1;
  pthread_create(&thread, NULL, func, (void *)id);

  //wait for func to change the value
  sleep(20);
  //assert that value was changed to 5
  assert(val==5);
}

int main(int argc, char **argv) {
  test_func();
}
