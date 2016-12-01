all: pabx_server pabx_caller pabx_rm

pabx_caller:
	gcc -o pabx_caller -pthread pabx_caller.c 

pabx_rm:
	gcc -o pabx_rm -pthread pabx_rm.c 

pabx_server:
	gcc -o pabx_server -pthread pabx_server.c 

clean:
	rm pabx_server pabx_caller pabx_rm
