#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>
#include <semaphore.h>

int rand_int(int n) {
	// Generate random integer number between [1, n]
	int r = rand();
	return (r % n) + 1;
}

int main(int argc, char **argv) {
	printf("Starting caller\n");
        
        //
        // OS -- OPEN NAMED SEMAPHORE HERE
        //      
        sem_t* semvar;
	
	semvar = sem_open("pabx",O_CREAT,0644,0);


	while(1) {
		// We will wait between 1-3 seconds before placing the next phone call
		int wait_time = rand_int(3);
		printf("Wait for %d seconds\n", wait_time);
		sleep(wait_time);
		printf("Access outside phone line\n");
		time_t t0 = time(NULL);
		// Try to get the line
                
                //
                // OS -- LOCK SEMAPHORE HERE
                //
                
		sem_wait(semvar);

		time_t t = time(NULL) - t0;
		// We get the phone line, using it for 1-5 seconds.
		int call_time = rand_int(5);
		printf("Get a phone line after waiting for %d seconds.  I will use for %d seconds.\n", t, call_time);
		sleep(call_time);
		// We hang up the phone

                //
                // OS -- UNLOCK SEMAPHORE HERE
                //

		sem_post(semvar);

		printf("Hang up the phone.\n");
	}
}
