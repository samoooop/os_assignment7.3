all: arena warrior_a warrior_b rm_sem

arena:
	gcc -o arena  arena.c -lpthread -lrt

warrior_a:
	gcc -o warrior_a warrior_a.c -lpthread  -lrt

warrior_b:
	gcc -o warrior_b warrior_b.c -lpthread -lrt

rm_sem:
	gcc -o rm_sem rm_sem.c -lpthread -lrt 

clean:
	rm warrior_a warrior_b arena rm_sem
