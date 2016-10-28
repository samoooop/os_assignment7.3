#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <semaphore.h>

#define RUNNING 0
#define END 1

struct shared_memory {
	char game_status;
	char buf[20];
};

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


	srand(time(NULL));
    char me = 'a';

    sem_t* semvar_a = sem_open("warrior a",O_CREAT,0644,0);
    sem_t* semvar_b = sem_open("warrior b",O_CREAT,0644,0);
    sem_t* semvar_arena = sem_open("arena",O_CREAT,0644,0);

	while(1) {
        sem_wait(semvar_a);
		if(shared_mem_ptr->game_status == END){
            break;
        }
        int write_index = rand()%20;
        int i;
        for(i=0;i<3;i++){
            if(shared_mem_ptr->buf[(write_index+i)%20]==' ' || shared_mem_ptr->buf[(write_index+i)%20]!=me){
                shared_mem_ptr->buf[(write_index+i)%20] = me;
                break;
            }
        }
        sem_post(semvar_arena);
	}
	if(close(fd_shm) == -1)
		error("close");
	sem_post(semvar_arena);
	//if(shm_unlink("pc") == -1)
	//	error("shm_unlink");
}

