#include "defns.h"

#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), connect(), sendto(), and recvfrom() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_addr() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */
#include <signal.h>
#include <sys/signal.h>
#define ECHOMAX 255     /* Longest string to echo */
#define ITERATIONS	5
int long t = 0;
static int alarm_value = 0;

void DieWithError(const char *errorMessage) /* External error handling function */
{
    perror(errorMessage);
    exit(1);
}

void TurnOffInterrupt(int signum)
{
	printf("Alarm turned off through CTRL+backslash button \n");

	// Turn off all pending alarms
	alarm(0);
}

//Function to turn OFF alarm.  It can be called externally from the application.

void quitalarm(void)
{
	printf("Alarm cancelled through Quitalrm() function");
	alarm(0);
}


void timerInterruptHandler(int a)
{
	printf("Timer completion : %ld \n", t);
	t++;

	// Set a new alarm for 1 second
	alarm(alarm_value);
}

void alarm_handler(int sigon)
{
	printf("alarm interrupt\n");
	//sendto method.
}




int main(int argc, char *argv[])
{
    int sock;                        /* Socket descriptor */
    struct sockaddr_in echoServAddr; /* Echo server address */
    struct sockaddr_in fromAddr;     /* Source address of echo */
    unsigned short echoServPort;     /* Echo server port */
    unsigned int fromSize;           /* In-out of address size for recvfrom() */
    char *servIP;                    /* IP address of server */
    char *echoString;                /* String to send to echo server */
    char echoBuffer[ECHOMAX+1];      /* Buffer for receiving echoed string */
    int echoStringLen;               /* Length of string to echo */
    int respStringLen;               /* Length of received response */
	char *clname;
	//freopen("outputcl.txt", "a", stdout);
	//freopen("errorcl.txt", "a", stderr);
    if (argc < 3)    /* Test for correct number of arguments */
    {
        fprintf(stderr,"Usage: %s <Server IP address> <Echo Port>\n", argv[0]);
        exit(1);
    }
	clname = argv[0];
    servIP = argv[1];           /* First arg: server IP address (dotted quad) */
    echoServPort = atoi(argv[2]);  /* Second arg: Use given port, if any */

	printf( "Arguments passed: server IP %s, port %d\n", servIP, echoServPort );

    /* Create a datagram/UDP socket */

    if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
        DieWithError("socket() failed");

    /* Construct the server address structure */

    memset(&echoServAddr, 0, sizeof(echoServAddr));    /* Zero out structure */
    echoServAddr.sin_family = AF_INET;                 /* Internet addr family */
    echoServAddr.sin_addr.s_addr = inet_addr(servIP);  /* Server IP address */
    echoServAddr.sin_port   = htons(echoServPort);     /* Server port */

	/* Pass string back and forth between server 5 times */

	printf( "Echoing %d iterations.\n", ITERATIONS );
	char str[20];
	char m1[24];
	int result1;
	result1 = gethostname(m1, 24);
	if (result1)
	{
		perror("gethostname");
		return EXIT_FAILURE;
	}
	printf("hostname is %s \n", m1);


	printf("Enter a value: \n");
	scanf("%s", str);
	printf("The script name is %s \n", str);
	FILE *fp;
	char *line = NULL;
	size_t len = 0;
	ssize_t read;

	struct request request1;
	int clc = strcmp(clname, "./client");
	//printf("name is %s ,compare result is %d\n", clname, clc);
	if (strcmp(clname, "./client") == 0) {
		request1.c = 0;
	}
	if (strcmp(clname, "./client2") == 0) {
		request1.c = 1;
	}
	

	request1.r = 0;
	request1.i = 0;
	
	char sendops[80];
	fp = fopen(str, "r");
	if (fp == NULL) {
		printf("error with file fopen.");
		exit(EXIT_FAILURE);
	}
	while ((read = getline(&line, &len, fp)) != -1) {
		printf("Retrieved line of length %zu :\n", read);
		if (line !="\n") {
			printf("%s", line);
			strcpy(sendops, line);
			char *q = strtok(sendops, " ");
			if (q != NULL) {
				printf("%s", q);
				//int k;
				//k = strcmp(q, "fail\r\n");
				//printf("k is %d\n", k);
				if (strcmp(q, "fail\r\n") != 0) {
					request1.r++;
					strcpy(request1.m, m1);
					strcpy(request1.operation, line);
					printf("Client sent string %s %d %d %d %s to server\n", request1.m, request1.c, request1.r, request1.i, request1.operation);
					int tempint = 0;
					
					do {
						tempint = sendto(sock, (struct request*)&request1, (1024 + sizeof(request1)), 0, (struct sockaddr *)&echoServAddr, sizeof(echoServAddr));
						if (tempint == -1) {
							DieWithError("sendto() sent a different number of bytes than expected\n");
						}
						
						printf("send request to server!\n");
						//alarm_value = 1;
						//signal(SIGALRM, timerInterruptHandler);
						//signal(SIGQUIT, TurnOffInterrupt);
						//alarm(alarm_value);
						fromSize = sizeof(fromAddr);
						//int s = 2;
						//while (s>1)
						//{
						//	if (t > 10)
						//	{
								//respStringLen = recvfrom(sock, echoBuffer, ECHOMAX, 0, (struct sockaddr *) &fromAddr, &fromSize);
						//		printf("Alarm cancelled through Quitalrm() function");
								//sendto(sock, (struct request*)&request1, (1024 + sizeof(request1)), 0, (struct sockaddr *)&echoServAddr, sizeof(echoServAddr));
						//		pause();   //t= The above printf statement works only if this pause is included.
						//		quitalarm();
						//	}
						//	pause;
						//	sendto(sock, (struct request*)&request1, (1024 + sizeof(request1)), 0, (struct sockaddr *)&echoServAddr, sizeof(echoServAddr));
						//	s--;
						//}
						memset(echoBuffer, 0, sizeof(echoBuffer));
						if ((respStringLen = recvfrom(sock, echoBuffer, ECHOMAX, 0, (struct sockaddr *) &fromAddr, &fromSize)) > ECHOMAX)
							DieWithError("recvfrom() failed");
						printf("Client received string from server: %s\n", echoBuffer);

					} while (respStringLen < 2);
						
				}else {
					request1.i++;
				}
			}else {
				printf("empty line.\n");

			}
		}
	}

	fclose(fp);


    
    close(sock);
    exit(0);
}