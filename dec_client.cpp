#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string>
#include <cstring>
#include <dirent.h>
#include <pwd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <semaphore.h>
//#include "Server.h"
using namespace std;
#define SIZE 2048
#define server_addr "192.168.1.2"
/**
 myhttpd web sever
 **/



int debug=0;
int port=9090;//default port

char* host="127.0.0.1";
char* serverDir=NULL;
int sleep_time=5; //scheduler sleep time
int main(int argc,char** argv){
	static struct sockaddr_in ser_addr;
	char buffer[SIZE];
	int i,sockfd;
	char * servername=NULL;
	int k,len;
	string buf;
	char* request;
	//char *server_addr;
	int ch;
	opterr=0;
	while((ch=getopt(argc,argv,"s:hp:"))!=-1){
		switch(ch){

		case'h':
		{
			printf("\n\tdec_client [−h] [-s server-host] [-p port-number]");
			printf("\n\n\tOPTIONS :");
			printf("\n\n\t−h : Print a usage summary with all options and exit.");
			printf("\n\t-s server-host : Connect to the specified host (by hostname or by IP address). If not provided, connect to the localhost.");
			printf("\n\t-p port-number : Connect to the server at the given port. If not provided, connect to 9090.");

			printf("\n");

			exit(0);
			break;
		}


		case's':
		{
			host=optarg;
			break;
		}
		case'p':
		{
			port=atoi(optarg);
			break;
		}
		default:
			printf("Unknown Options!");
			break;
		}
	}
	sockfd = socket(AF_INET, SOCK_STREAM,0);

	if(sockfd ==-1)
	{
		printf("Cannot create socket \n");
		return 0;
	}
	//cout<<"port is="<<port;
	ser_addr.sin_family = AF_INET;
	ser_addr.sin_addr.s_addr = inet_addr(host);
	ser_addr.sin_port = htons(port);
	char* buf1;
	char data[1024];
	int t_bytes;
	char c='\n';

	if(connect(sockfd, (struct sockaddr *)&ser_addr, sizeof(ser_addr)) !=0)
	{
		printf("Unable to connect to server");
		close(sockfd);
	}
	while(1){
		getline(cin,buf);
		int n=buf.size();
		request=new char[n+1];
		for(int i=0;i<n;i++){
			request[i]=buf[i];
		}
		request[n]='\0';
		send(sockfd,request,strlen(request),0);
		t_bytes = recv(sockfd,data,1000,0);
		data[t_bytes]='\0';
		if(t_bytes==0) {
			cout << "Server closed";
			exit(1);
		}
		if(t_bytes==-1)
			cout << "Error";
		cout << data;
	}

	close(sockfd);
	printf("client disconnected\n");

	return 0;
}
