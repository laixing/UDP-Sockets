#The pond symbol is used for writing the comments.

#make file overview :--	
#you can add a little description here.

#variable declaration :-
cc=gcc
MAKE=make
RM =rm

#targets .
server: UDPEchoServer.c 
	$(cc) ~/UDPEchoServer.c -o server 
	./server 33333
	sleep 10s
	
	
#another target for client

client: UDPEchoClient.c	
	$(cc) ~/UDPEchoClient.c -o client
	./client 129.219.10.241 33333
	sleep 10s 

client2: 
	$(cc) ~/UDPEchoClient.c -o client2
	./client2 129.219.10.241 33333
	sleep 10s  	
	
clean:
	#$(RM) server
	#$(RM) client
	@if [ -e server ]; then $(RM) server; fi
	@if [ -e client ]; then $(RM) client; fi	
	
