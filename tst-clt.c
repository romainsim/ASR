#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include<fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include<stdio.h>
#include<sys/un.h>
#include<arpa/inet.h>
int main(int argc,char** argv) {
	int id;
	char chaine[255];
	char sendd[255];
	char recvv[255];
	struct sockaddr_in sinfour;
	bzero(&sinfour,sizeof(sinfour));
	if(argc!=3) {
		printf("Port and ipv4 adress to connect serve \n");
		exit(0);
	}
	sinfour.sin_family = AF_INET;
	sinfour.sin_port = htons((short)strtol(argv[2],NULL,0));
	if(inet_pton(AF_INET,argv[1],&sinfour.sin_addr.s_addr) <= 0) {
			perror("inet_pton");
			exit(1);
	}
	id=socket(AF_INET,SOCK_STREAM,0);
	if(id==-1) {
		perror("probleme socket");
		exit(0);
	}
	if(connect(id,(struct sockaddr *)&sinfour,sizeof(sinfour)) ==-1) {
	perror("connect");
	exit(-1);
	}
			bzero(recvv,sizeof(recvv));
			recv(id,recvv,sizeof(recvv),0);
			printf("%s\n",recvv);
			bzero(chaine,sizeof(chaine));
			read(0,chaine,sizeof(chaine));
			send(id,chaine,sizeof(chaine),0);
			bzero(recvv,sizeof(recvv));
			recv(id,recvv,sizeof(recvv),0);
			printf("%s\n",recvv);
	close(id);
	exit(0);
}
