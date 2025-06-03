#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "user/thread.h"

int thread_create(void *(*start_routine)(void *), void *arg) {
    void *stack = malloc(4096);
    if(stack == (void *)-1)
        return -1;

    int pid = clone(stack + 4096);

    if (pid < 0) {
        free(stack);
        return -1;
    }

    if (pid == 0) {
        start_routine(arg);
        exit(0);
    }

    return 0;
}

void lock_init(struct lock_t* lock) {
    lock->locked = 0;
}

void lock_acquire(struct lock_t* lock) {
    while (__sync_lock_test_and_set(&lock->locked, 1) != 0)
        ; 
    __sync_synchronize();
}

void lock_release(struct lock_t* lock) {
    __sync_synchronize();
    __sync_lock_release(&lock->locked);
}
