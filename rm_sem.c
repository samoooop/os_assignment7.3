#include <stdio.h>
#include <semaphore.h>

void main() {
    int r = sem_unlink("arena");
    r = sem_unlink("warrior a");
    r = sem_unlink("warrior b");
}
