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

	// Initialize the shared memory
	shared_mem_ptr->front = 0;
	shared_mem_ptr->size = 0;
	shared_mem_ptr->status = EMPTY;
	int i;
	for(i=0 ; i < 20 ; i++) {
		char data = 'a' + i;
		printf("Producing '%c' and place it at %d\n", data,(shared_mem_ptr->front+shared_mem_ptr->size)%10);

		// pushing data to circular queue
		(shared_mem_ptr->buf)[(shared_mem_ptr->front+shared_mem_ptr->size)%10] = data;
		shared_mem_ptr->size++;
		shared_mem_ptr->status=shared_mem_ptr->size==10?FULL:FREE;
	
		while(shared_mem_ptr->status == FULL) {
			printf("Buffer is Full!\n");
			sleep(1);
		}
	}
	// Inform consumer to end
	shared_mem_ptr->status = END;
	if(close(fd_shm) == -1)
		error("close");
}

