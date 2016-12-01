#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>

#define EMPTY 0
#define FULL 1
#define FREE -1
#define END 2

struct shared_memory {
	int status;
	char buf[10];
	int front;
	int size;
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


	while(1) {
		while(shared_mem_ptr->status == EMPTY) {
			printf("Waiting for producer\n");
			sleep(1);
		}
		if(shared_mem_ptr->status == END && shared_mem_ptr->size == 0)
			break;

		// Pop data from queue
		
		char data = shared_mem_ptr->buf[shared_mem_ptr->front];
		shared_mem_ptr->front++;
		shared_mem_ptr->front%=10;
		shared_mem_ptr->size--;
		if(shared_mem_ptr->status != END){
			if(shared_mem_ptr->size == 0)
				shared_mem_ptr->status = EMPTY;
			else
				shared_mem_ptr->status = FREE;
		}
		
		printf("Consuming '%c'\n", data);
		// We don't want to consume too fast
		sleep(1);
	}

	if(close(fd_shm) == -1)
		error("close");
	if(shm_unlink("pc") == -1)
		error("shm_unlink");
}

