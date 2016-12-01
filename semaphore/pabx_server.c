#include <stdio.h>
#include <fcntl.h>
#include <semaphore.h>

int main(int argc, char **argv) {
	int num_lines = 2;
	if(argc > 1)
		num_lines = atoi(argv[1]);
	printf("Starting PABX with %d phone lines.\n", num_lines);

	//
	// OS -- CRAETE NAMED SEMAPHORE HERE
	//

	//key_t semkey = 12345218379;
	sem_t* semvar;
	int semval=0;

	semvar = sem_open("pabx",O_CREAT,0644,num_lines);
	printf("asdasd - >>> %x\n",0644);
	//printf("%d\n",semvar);

	//sem_post(semvar);
	//int i=0;
	//for(i=0;i<num_lines;i++)
	//	sem_post(semvar);
 
	//sem_getvalue(semvar,&semval);
	//printf("%d\n",semval);
	//semwait(&semvar);
	//int semval;
	while(1) {

		//
		// OS -- PLACE CURRENT VALUE OF SEMAPHORE IN 'semval' HERE
		//
		//semvar = sem_open("1234",0);;
		sem_getvalue(semvar,&semval);
		printf("There are %d phone lines available.\n", semval);
		sleep(3);
	}
}
