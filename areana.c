#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <time.h>
#include <stdlib.h>

#define RUNNING 0
#define END 1

struct shared_memory {
	char game_status;
	char buf[20];
};

void print_current_buffer(struct shared_memory*shared_mem_ptr){
        printf("Current Buffer = [ ");
        for(int i=0;i<20;i++){
            printf("%c ",shared_mem_ptr->buf[i]);
        }
        printf("]\n");
}

int main() {
	struct shared_memory *shared_mem_ptr;
	int fd_shm;

	// Get shared memory 
	if ((fd_shm = shm_open ("pc", O_RDWR | O_CREAT, 0666)) == -1)
		error ("shm_open");

	if (ftruncate (fd_shm, sizeof (struct shared_memory)) == -1)
		error ("ftruncate");

	if ((shared_mem_ptr = mmap (NULL, sizeof (struct shared_memory), PROT_READ | PROT_WRITE, MAP_SHARED,
		fd_shm, 0)) == MAP_FAILED)
		error ("mmap");

	// Initialize the shared memory and semaphore
	for(int i=0;i<20;i++){
        shared_mem_ptr->buf[i] = ' ';
    }

    sem_t* semvar_a = sem_open("warrior a",O_CREAT,0644,0);
    sem_t* semvar_b = sem_open("warrior b",O_CREAT,0644,0);
    sem_t* semvar_arena = sem_open("arena",O_CREAT,0644,0);

    srand(time(NULL));

	int i;
	for(i=0 ; i < 20 ; i++) {
        printf("Round-%d:",i+1);
        print_current_buffer(shared_mem_ptr);
        int writer = rand()%2;
        if(writer == 0){
            printf("Let warrior a enter arena\n");
            sem_post(semvar_a);
            sem_wait(semvar_arena);
            printf("Warrior a exits arena\n");
        }else{
            printf("Let warrior b enter arena\n");
            sem_post(semvar_b);
            sem_wait(semvar_arena);
            printf("Warrior b exits arena\n");
        }
        print_current_buffer(shared_mem_ptr);
    }	
	// Inform consumer to end
	shared_mem_ptr->status = END;
	if(close(fd_shm) == -1)
		error("close");
}
