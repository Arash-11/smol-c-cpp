/*
 * experiment: locking in thread and forking
*/

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void* thandler(void* arg) {
    (void)arg;
    pthread_mutex_lock(&lock);
    sleep(3);
    pthread_mutex_unlock(&lock);
    return NULL;
}

int main(void) {
    pthread_t t1;

    pthread_create(&t1, NULL, thandler, NULL);
    // pthread_join(t1, NULL);

    // sleep(1);

    pid_t rc = fork();
    // skipping error checking
    if (rc == 0) {
        const char msg[] = "[child] about to hold lock...\n";
        write(STDOUT_FILENO, msg, sizeof(msg) - 1);
        sleep(5);
        if (pthread_mutex_trylock(&lock) == EBUSY) {
            printf("[child] could not hold lock. Lock was busy\n");
        } else {
            printf("[child] lock acquired!\n");
        }
    } else { // in parent
        if (waitpid(rc, 0, 0) == -1) {
            perror("waitpid error");
            exit(1);
        }
    }

    return 0;
}
