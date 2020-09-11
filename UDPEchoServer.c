#include "defns.h"

#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket() and bind() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_ntoa() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */
#include <ctype.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>
#include <sys/file.h>

#define ECHOMAX 255     /* Longest string to echo */

void DieWithError(const char *errorMessage) /* External error handling function */
{
    perror(errorMessage);
    exit(1);
}

bool file_exists(const char * filename) {
	FILE *file;
	file = fopen(filename, "r");
	if (file!=NULL) {
		fclose(file);
		return true;
	}
	return false;
}

int main(int argc, char *argv[])
{
    int sock;                        /* Socket */
    struct sockaddr_in echoServAddr; /* Local address */
    struct sockaddr_in echoClntAddr; /* Client address */
    unsigned int cliAddrLen;         /* Length of incoming message */
    char echoBuffer[ECHOMAX];        /* Buffer for echo string */
    unsigned short echoServPort;     /* Server port */
    int recvMsgSize;                 /* Size of received message */
	FILE * pFile;
	int R;
	bool isold;
	int fd;
	int nread;
	FILE *Filelock;
	struct request *temp = malloc(sizeof(struct request));
	char echoBuffer3[ECHOMAX];

	//freopen("output.txt", "a", stdout);
	//freopen("error.txt", "a", stderr);
    if (argc != 2)         /* Test for correct number of parameters */
    {
        fprintf(stderr,"Usage:  %s <UDP SERVER PORT>\n", argv[0]);
        exit(1);
    }

    echoServPort = atoi(argv[1]);  /* First arg:  local port */

    /* Create socket for sending/receiving datagrams */
    if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
        DieWithError("socket() failed\n");

    /* Construct local address structure */
    memset(&echoServAddr, 0, sizeof(echoServAddr));   /* Zero out structure */
    echoServAddr.sin_family = AF_INET;                /* Internet address family */
    echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
    echoServAddr.sin_port = htons(echoServPort);      /* Local port */

    /* Bind to the local address */
    if (bind(sock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0)
        DieWithError("bind() failed\n");
  
    for (;;) /* Run forever */
    {	
		
        /* Set the size of the in-out parameter */
        cliAddrLen = sizeof(echoClntAddr);

		if ((recvMsgSize = recvfrom(sock, temp, sizeof(*temp), 0, (struct sockaddr *) &echoClntAddr, &cliAddrLen)) < 0) {
			DieWithError("recvfrom() failed\n");
		}
        /* Block until receive message from a client */
		//if ((recvMsgSize = recvfrom(sock, echoBuffer, ECHOMAX, 0, (struct sockaddr *) &echoClntAddr, &cliAddrLen)) >= 0) {
		//	sendto(sock, echoBuffer3, strlen(echoBuffer3), 0, (struct sockaddr *) &echoClntAddr, sizeof(echoClntAddr));

		//}
             

	//echoBuffer[ recvMsgSize ] = '\0';

        printf("Server handling client %s\n", inet_ntoa( echoClntAddr.sin_addr ));
    //    printf("Server receives string: %s\n", echoBuffer );
		printf("Received: %s %d %d %d %s \n", temp->m, temp->c, temp->r, temp->i, temp->operation);
		int n_spaces = 0;
		char recops[80];
		char clinf[80];
		char ctep[10];
		char rtep[10];
		char itep[10];
		char  buf7[15];
		char buf8[15];
		char echoBufferold[20];
		char echoBuffer2[20];
		char echoBuffer4[ECHOMAX];
		char echoBuffer5[ECHOMAX];
		sprintf(ctep, " %d", temp->c);
		sprintf(rtep, " %d", temp->r);
		sprintf(itep, " %d", temp->i);
		char lockinf[80];
		strcpy(lockinf, temp->m);
		strcat(lockinf, ctep);
		
		if (!file_exists("cliTable.txt")) {
			R = -1;
			FILE * fp2;
			fp2 = fopen("cliTable.txt", "a");
			fclose(fp2);
		}
		if (file_exists("cliTable.txt")) {
			FILE *fpc;
			char *line = NULL;
			size_t len = 0;
			ssize_t readc;
			char clstr[80];
			char machine[10];
			int ctab;
			int rtab;
			int itab;
			isold = false;
			fpc = fopen("cliTable.txt", "r+");
			if (fpc == NULL) {
				printf("error with file fopen");
				exit(EXIT_FAILURE);
			}
			while ((readc = getline(&line, &len, fpc)) != -1) {
				printf("Retrieved line of length %zu :\n", readc);
				if (line != "\n") {
					printf("%s", line);
					strcpy(clstr, line);
					char * cl = strtok(clstr, " ");
					if (cl != NULL) {
						printf("%s\n", cl);
						strcpy(machine, cl);
						printf("the vm machine name is:%s\n", machine);
						cl = strtok(NULL, " ");
						if (cl != NULL) {
							printf("%s\n", cl);
							sscanf(cl, "%d", &ctab);
							cl = strtok(NULL, " ");
							if (cl != NULL) {
								printf("%s\n", cl);
								sscanf(cl, "%d", &rtab);
								R = rtab;
								cl = strtok(NULL, " ");
								if (cl != NULL) {
									printf("%s\n", cl);
									sscanf(cl, "%d", &itab);
									/*	printf("itab is %d\n", itab);
										int ii = atoi(itep);
										printf("ii is %d\n", ii);
										if (ii == itab) {
											printf("they are equal!\n");
										}
										else {
											printf("they are not equal!\n");
										}*/



								}
							}
						}
					}
					if ((strcmp(machine, temp->m) == 0) && (ctab == atoi(ctep)) && (rtab == atoi(rtep)) && (itab == atoi(itep))) {
						printf("the client is old!\n");
						isold = true;
						
					}
					
				}

			}
			fclose(fpc);
			printf("the value of isold is %d\n",isold);
			if (isold) {
				
				strcpy(echoBufferold, "stored");
				if (sendto(sock, echoBufferold, strlen(echoBufferold), 0, (struct sockaddr *) &echoClntAddr, sizeof(echoClntAddr)) != strlen(echoBufferold)) {
					DieWithError("sendto() sent a different number of bytes than expected");
				}
				printf("sent message to acquire another request!\n");


			}
			if (!isold) {
				printf("the client is new!\n");
				printf("R is %d and rtep is %d\n", R,atoi(rtep));
				if (R>atoi(rtep)) {
					char echoBuffer1[20];
					strcpy(echoBuffer1, "igored!");
					if (sendto(sock, echoBuffer1, strlen(echoBuffer1), 0, (struct sockaddr *) &echoClntAddr, sizeof(echoClntAddr)) != strlen(echoBuffer1)) {
						DieWithError("sendto() sent a different number of bytes than expected");
					}

				}
				if (R == atoi(rtep)) {
					
					strcpy(echoBuffer2, "stored");
					if (sendto(sock, echoBuffer2, strlen(echoBuffer2), 0, (struct sockaddr *) &echoClntAddr, sizeof(echoClntAddr)) != strlen(echoBuffer2)) {
						DieWithError("sendto() sent a different number of bytes than expected");
					}
						   

				}
				if (R < atoi(rtep)) {
					int r;
					r = rand() % 3+1;
					printf("rand() is %d\n", r);
					if (r == 1) {

						printf("igored!\n");
						
						strcpy(echoBuffer5, "");
						if (sendto(sock, echoBuffer5, strlen(echoBuffer5), 0, (struct sockaddr *) &echoClntAddr, sizeof(echoClntAddr)) != strlen(echoBuffer5)) {
							DieWithError("sendto() sent a different number of bytes than expected");
						}

					}
					if (r == 2) {
						printf("perform the request and drop the reply!\n");
						strcpy(recops, temp->operation);
						printf("received operation: %s \n", recops);
						char *p1 = strtok(recops, " ");
						int i1 = 0;
						char file1[15];
						char model1[15];
						

						if (p1 != NULL) {
							printf("result is: %s\n ", p1);

						}
						if (strcmp(p1, "open") == 0) {
							printf("do open operation!\n");
							p1 = strtok(NULL, " ");
							if (p1 != NULL) {
								printf("%s\n", p1);
								strcpy(file1, p1);
								printf("filepath is %s\n", file1);
								p1 = strtok(NULL, " ");
								if (p1 != NULL) {
									printf("%s\n", p1);
									strcpy(model1, p1);
									printf("open model is %s\n", model1);
								}
							}
							else {
								printf("no filepath provided.\n");
							}
							printf("we now have the value %s!", model1);
							int judge = strcmp(model1, "write\r\n");
							//printf("value of judge is %d\n", judge); 
							if (strcmp(model1, "write\r\n") == 0) {
								printf("open the file with write model.\n");
								//pFile = fopen(file1, "w+");
								fd = open(file1, O_WRONLY);
								printf("open with write\n");
								
								Filelock = fopen("lock.txt", "a");
								strcat(lockinf, "write");
								strcat(lockinf, "\n");
								fputs(lockinf, Filelock);

								fclose(Filelock);
								printf("add locker information to lock.txt.\n");
							}
							else if (strcmp(model1, "read\r\n") == 0) {
								printf("open the file with read model.\n");
								//pFile = fopen(file1, "r+");
								fd = open(file1, O_RDONLY);
								printf("open with read\n");
								Filelock = fopen("lock.txt", "a");
								strcat(lockinf, "read");
								strcat(lockinf, "\n");
								fputs(lockinf, Filelock);

								fclose(Filelock);
								printf("add locker information to lock.txt.\n");
							}
							else if (strcmp(model1, "readwrite\r\n") == 0) {
								printf("open the file with readwrite model.\n");
								//pFile = fopen(file1, "a+");
								fd = open(file1, O_RDWR);
								printf("open with write\n");
								Filelock = fopen("lock.txt", "a");
								strcat(lockinf, "readwrite");
								strcat(lockinf, "\n");
								fputs(lockinf, Filelock);

								fclose(Filelock);
								printf("add locker information to lock.txt.\n");
							}
						}
						else if (strcmp(p1, "read") == 0) {
							printf("do read operation!\n");
							p1 = strtok(NULL, " ");
							if (p1 != NULL) {
								printf("%s\n", p1);
								strncpy(file1, p1, sizeof(file1));
								printf("filepath is %s\n", file1);
								p1 = strtok(NULL, " ");
								if (p1 != NULL) {
									printf("%s\n", p1);
									int num1;
									sscanf(p1, "%d", &num1);
									printf("the value of num:%d\n", num1);
									
									
									//if (fd == NULL) {
									//	perror("Error opening file\n");
									//}
									
									
									nread = read(fd, &buf7, num1);
									printf("nread is %d\n",nread);
									
									printf("the result of read operation is %s", buf7);
									//int count = 0;
									//int ch;
									//while ((ch = fgetc(pFile)) != EOF || count < num1) {
									//	printf("%c", ch);
									//	count++;
									//}

									printf("\n read operation finished.\n");

								}
							}
							else {
								printf("no filepath provided.\n");
							}
						}
						else if (strcmp(p1, "write") == 0) {
							printf("do write operation!\n");
							p1 = strtok(NULL, " ");
							if (p1 != NULL) {
								char file2[15];
								char * str1;
								int num2;
								printf("%s\n", p1);
								strcpy(file2, p1);
								printf("filepath is %s\n", file2);
								p1 = strtok(NULL, " ");
								if (p1 != NULL) {
									printf("%s\n", p1);
									//int num2;
									//sscanf(q, "%d", &num2);
									//strncpy(str1, q, sizeof(q));
									//strncpy(str1, q,sizeof(q));
									//printf("num2 is %d:", num2);
									int nwrite1;
									nwrite1 = write(fd, p1, sizeof(p1) - 1);
									printf("the return value of write: %d\n", nwrite1);



								}
							}
						}
						else if (strcmp(p1, "lseek") == 0) {
							printf("do lseek operation!\n");
							p1 = strtok(NULL, " ");
							if (p1 != NULL) {
								printf("%s\n", p1);
								strncpy(file1, p1, sizeof(file1));
								printf("filepath is %s\n", file1);
								p1 = strtok(NULL, " ");
								if (p1 != NULL) {
									printf("%s\n", p1);
									int num2;
									sscanf(p1, "%d", &num2);
									printf("the value of num:%d\n", num2);
									//if (fd == NULL) {
									//	perror("Error opening file\n");
									//}
									printf("the filepath is: %s\n", file1);
									int nlseek1;
									nlseek1 = lseek(fd, num2, SEEK_SET);
									printf("the new file offset is: %d\n", nlseek1);


								}
								printf("lseek operation finished.\n");

							}
						}
						else if (strcmp(p1, "close") == 0) {
							printf("do close operation!\n");
							int nclose1;
							nclose1 = close(fd);
							printf("the return code for close is: %d\n", nclose1);
							


						}











						strcpy(clinf, temp->m);
						strcat(clinf, ctep);
						strcat(clinf, rtep);
						strcat(clinf, itep);
						strcat(clinf, "\n");
						printf("the information stored in client table is %s\n", clinf);



						FILE * table1;
						table1 = fopen("cliTable.txt", "a");
						if (table1 != NULL) {
							fputs(clinf, table1);
							fclose(table1);
						}
						
						strcpy(echoBuffer4, " ");
						if (sendto(sock, echoBuffer4, strlen(echoBuffer4), 0, (struct sockaddr *) &echoClntAddr, sizeof(echoClntAddr)) != strlen(echoBuffer4)) {
							DieWithError("sendto() sent a different number of bytes than expected");
						}
					}
					if (r == 3) {
						printf("perform the request and send responds to the client!\n");
						strcpy(recops, temp->operation);
						printf("received operation: %s \n", recops);
						char *p = strtok(recops, " ");
						int i = 0;
						char file3[15];
						char model3[15];
						

						if (p != NULL) {
							printf("result is: %s\n ", p);

						}
						if (strcmp(p, "open") == 0) {
							printf("do open operation!\n");
							p = strtok(NULL, " ");
							if (p != NULL) {
								printf("%s\n", p);
								strcpy(file3, p);
								printf("filepath is %s\n", file3);
								p = strtok(NULL, " ");
								if (p != NULL) {
									printf("%s\n", p);
									strcpy(model3, p);
									printf("open model is %s\n", model3);
								}
							}
							else {
								printf("no filepath provided.\n");
							}
							printf("we now have the value %s!", model3);
							int judge = strcmp(model3, "write\r\n");
							//printf("value of judge is %d\n", judge); 
							if (strcmp(model3, "write\r\n") == 0) {
								printf("open the file with write model.\n");
								//pFile = fopen(file1, "w+");
								fd = open(file3, O_WRONLY);
								
								printf("open with write\n");
								Filelock = fopen("lock.txt", "a");
								strcat(lockinf, "write");
								strcat(lockinf, "\n");

								fputs(lockinf, Filelock);

								fclose(Filelock);

							}
							else if (strcmp(model3, "read\r\n") == 0) {
								printf("open the file with read model.\n");
								//pFile = fopen(file1, "r+");
								fd = open(file3, O_RDONLY);
								printf("open with read\n");
								Filelock = fopen("lock.txt", "a");
								strcat(lockinf, "read");
								strcat(lockinf, "\n");
								fputs(lockinf, Filelock);

								fclose(Filelock);
							}
							else if (strcmp(model3, "readwrite\r\n") == 0) {
								printf("open the file with readwrite model.\n");
								//pFile = fopen(file1, "a+");
								fd = open(file3, O_RDWR);
								printf("open with write\n");
								Filelock = fopen("lock.txt", "a");
								strcat(lockinf, "readwrite");
								strcat(lockinf, "\n");
								fputs(lockinf, Filelock);

								fclose(Filelock);
							}
						}
						else if (strcmp(p, "read") == 0) {
							printf("do read operation!\n");
							p = strtok(NULL, " ");
							if (p != NULL) {
								printf("%s\n", p);
								strncpy(file3, p, sizeof(file3));
								printf("filepath is %s\n", file3);
								p = strtok(NULL, " ");
								if (p != NULL) {
									printf("%s\n", p);
									int num3;
									sscanf(p, "%d", &num3);
									printf("the value of num:%d\n", num3);
									
									
									
									
									nread = read(fd, &buf8, num3);
									
									printf("the result of read operation is %s",buf8);
									//int count = 0;
									//int ch;
									//while ((ch = fgetc(pFile)) != EOF || count < num1) {
									//	printf("%c", ch);
									//	count++;
									//}

									printf("\n read operation finished.\n");

								}
							}
							else {
								printf("no filepath provided.\n");
							}
						}
						else if (strcmp(p, "write") == 0) {
							printf("do write operation!\n");
							p = strtok(NULL, " ");
							if (p != NULL) {
								char file4[15];
								char * str1;
								int num4;
								printf("%s\n", p);
								strcpy(file4, p);
								printf("filepath is %s\n", file4);
								p = strtok(NULL, " ");
								if (p != NULL) {
									printf("%s\n", p);
									//int num2;
									//sscanf(q, "%d", &num2);
									//strncpy(str1, q, sizeof(q));
									//strncpy(str1, q,sizeof(q));
									//printf("num2 is %d:", num2);
									int nwrite3;
									nwrite3 = write(fd, p, sizeof(p) - 1);
									printf("the return value of write: %d\n", nwrite3);



								}
							}
						}
						else if (strcmp(p, "lseek") == 0) {
							printf("do lseek operation!\n");
							p = strtok(NULL, " ");
							if (p != NULL) {
								printf("%s\n", p);
								strncpy(file3, p, sizeof(file3));
								printf("filepath is %s\n", file3);
								p = strtok(NULL, " ");
								if (p != NULL) {
									printf("%s\n", p);
									int num3;
									sscanf(p, "%d", &num3);
									printf("the value of num:%d\n", num3);
									if (pFile == NULL) {
										perror("Error opening file\n");
									}
									printf("the filepath is: %s\n", file3);
									int nlseek3;
									nlseek3 = lseek(fd, num3, SEEK_SET);
									printf("the new file offset is: %d\n", nlseek3);


								}
								printf("lseek operation finished.\n");

							}
						}
						else if (strcmp(p, "close") == 0) {
							printf("do close operation!\n");
							int nclose3;
							nclose3 = close(fd);
							printf("the return code for close is: %d\n", nclose3);
							//remove("lock.txt");
							//Filelock = fopen("lock.txt", "a");
							//fclose(Filelock);
						}



						strcpy(clinf, temp->m);
						strcat(clinf, ctep);
						strcat(clinf, rtep);
						strcat(clinf, itep);
						strcat(clinf, "\n");
						printf("the information stored in client table is %s\n", clinf);


						FILE * table2;
						table2 = fopen("cliTable.txt", "a");
						if (table2 != NULL) {
							fputs(clinf, table2);
							fclose(table2);
						}

						
						strcpy(echoBuffer3, "completed with case 3!\n");
						if (sendto(sock, echoBuffer3, strlen(echoBuffer3), 0, (struct sockaddr *) &echoClntAddr, sizeof(echoClntAddr)) != strlen(echoBuffer3)) {
							DieWithError("sendto() sent a different number of bytes than expected");
						}

					}
					


				}

				

			}
			



		}
		
		
    }
    /* NOT REACHED */
}