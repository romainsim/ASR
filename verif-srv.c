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
#include<time.h>
int function(int x,int y) {
		return (x-1)*y+2;
}
int main(int argc,char** argv) {
	int id;
	int xrand,yrand;
	int expected;
	int nid;
	int yes = 1;
	int x = 0;
	char chaine[50];
	char reponse[50];
	struct sockaddr_in sinfour;
	if(argc!=3) {
			printf("You need ipv4 adress and port");
			exit(0);
		}
	bzero(&sinfour,sizeof(sinfour));
	sinfour.sin_family = AF_INET;
	sinfour.sin_port = htons((short)strtol(argv[2],NULL,0));
	if(inet_pton(AF_INET,argv[1],&sinfour.sin_addr.s_addr) <= 0) {
			perror("inet_pton");
			exit(1);
	}
	id=socket(AF_INET,SOCK_STREAM,0);
		
	if(id==-1) {
		perror("socket");
		exit(-1);
	}
	setsockopt(id,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int));
	int addrlen;
	addrlen = sizeof(sinfour);
	if(bind(id,(struct sockaddr *)&sinfour,sizeof(sinfour))==-1) {
		perror("bind");
		exit(-2);
	}
	if(listen(id,0)==-1){
		perror("listen");
		exit(-3);
	}
	while(1) {
		if((nid=accept(id,(struct sockaddr *)&sinfour,&addrlen))==-1) {
			perror("probleme accept");
			exit(0);
		}
		x=fork();
		if(x==-1) {
			perror("erreur de forkation");
			exit(0);
		}
		if(x==0) {
			/*code du fils*/
			close(id);
			bzero(chaine,sizeof(chaine));
			srand(time(NULL)^ (getpid()<<16));/*Making a new random range*/
			xrand=rand()%5+1;
			yrand=rand()%25 + 1;
			sprintf(chaine,"(%d-1)*%d+2",xrand,yrand);
			send(nid,chaine,sizeof(chaine),0);
			expected=function(xrand,yrand);
			recv(nid,reponse,sizeof(reponse),0);
			if(strtol(reponse,NULL,0) == expected) {
			bzero(chaine,sizeof(chaine));
			strcpy(chaine,"test passed succesfully");
			}else {
			strcpy(chaine,"test failed. bye");
			}
			send(nid,chaine,sizeof(chaine),0);
			exit(0);
		}
		if(x>0) {
			/* code du pere*/
			close(nid);
		}
	}
}
