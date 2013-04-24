//============================================================================
// Name        : OS_DEC.cpp
// Author      : Puneet Arora
//============================================================================
#include <cstdlib>
#include <exception>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fstream>
#include <pthread.h>
#include <time.h>
#include <sstream>
#include <iomanip>
#include <string>
#include <semaphore.h>
#include <string.h>
#include <dirent.h>
#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include <fstream>
using std::ofstream;

#include <cstdlib>
using namespace std;
fd_set master;
char buffer[256]="";
int port=9090;
int flag=0;
char* logFile="log.txt";


typedef struct graph *graph1;

typedef struct struct_vertex {
	char element;
	int visited;
	struct struct_edge *edges;
	struct struct_vertex *next;
} vert;

typedef struct struct_edge {
	char element;
	struct struct_vertex *connectsTo;
	struct struct_edge *next;
} edgtype;

typedef struct node {
	char fromelement;
	char toelement;
	int idnt;
	struct node *next;
} nodeT;

vert* search_vertex(char , vert **);

vert *head = NULL;
vert *curr = NULL;

nodeT *head_node = NULL;
nodeT *curr_node = NULL;


nodeT* create_list(char fromelement, char toelement, int idnt1)
{

	nodeT *ptr = (nodeT*)malloc(sizeof(nodeT));
	if(NULL == ptr)
	{
		printf("\n Out of Memory \n");
		return NULL;
	}
	ptr->fromelement = fromelement;
	ptr->toelement = toelement;
	ptr->idnt = idnt1;
	ptr->next = NULL;

	head_node = curr_node = ptr;
	return ptr;
}

nodeT* add_to_list(char fromelement, char toelement, int idnt1)
{
	if(NULL == head_node)
	{
		return (create_list(fromelement, toelement,idnt1));
	}


	nodeT *ptr = (nodeT*)malloc(sizeof(nodeT));
	if(NULL == ptr)
	{
		printf("\n Out of memory \n");
		return NULL;
	}
	ptr->fromelement = fromelement;
	ptr->toelement = toelement;
	ptr->idnt = idnt1;
	ptr->next = NULL;
	curr_node->next = ptr;
	curr_node = ptr;
	return ptr;
}



int del_vertex(char element)
{

	if(head==NULL)
	{
		//		printf("\n\nEmpty Linked List.Cant Delete The Data.");
		return 0;
	}
	else
	{
		vert *old,*temp;
		int flag=0;
		temp=head;
		while(temp!=NULL)
		{
			if(temp->element==element)
			{
				if(temp==head)    
					head=temp->next;  
				else
					old->next=temp->next;
				free(temp);
				flag=1;
			}
			else
			{
				old=temp;
				temp=temp->next;
			}
		}
		if(flag==0){
			//			printf("\nData Not Found...");
			return 0;
		}
		else{
			//			printf("\nData Deleted...Tap a key to continue");
			return 1;
		}


	}


}



int del_edge(char fromelement,char edg)
{
	vert* vtex=search_vertex(fromelement,NULL);
	if(vtex==NULL)
	{
		//		printf("\n\nVertex NOT FOUND.");
		return 0;
	}
	else
	{
		edgtype *old,*temp;
		int flag=0;
		temp=vtex->edges;
		while(temp!=NULL)
		{
			if(temp->element==edg)
			{
				if(temp==vtex->edges)        
					vtex->edges=temp->next;  
				else
					old->next=temp->next;
				free(temp);
				flag=1;
			}
			else
			{
				old=temp;
				temp=temp->next;
			}
		}
		if(flag==0){
			//		printf("\nEDGE Not Found...");
			return 0;
		}
		else{
			//		printf("\nEDGE Deleted...Tap a key to continue");
			return 1;
		}
	}

}

void revoke(){

	if(head_node==NULL)
	{
		//	printf("\n\nEmpty Linked List.Cant Delete The Data.");

	}
	else
	{

		nodeT *temp;
		int flag=0;
		temp=head_node;
		while(temp!=NULL)
		{
			if(temp->idnt==1){
				del_vertex(temp->fromelement);
			}
			else if(temp->idnt==2){
				del_edge(temp->fromelement,temp->toelement);
			}
			else{
				//			printf("invalid identity");
			}
			temp=temp->next;
		}
	}


}





vert* create_graph(char element,int visit)
{
	//printf("\ncreating the graph\n");
	vert *ptr = (vert*)malloc(sizeof(vert));
	if(NULL == ptr)
	{
		printf("\n Out of Memory \n");
		return NULL;
	}
	ptr->element = element;
	ptr->visited = visit;
	ptr->next = NULL;
	ptr->edges = NULL;

	head = curr = ptr;
	return ptr;
}

vert* add_vertex(char element,int visit, bool add_to_end)
{
	if(NULL == head)
	{
		return (create_graph(element,visit));
	}


	vert *ptr = (vert*)malloc(sizeof(vert));
	if(NULL == ptr)
	{
		printf("\n Out of memory \n");
		return NULL;
	}
	ptr->element = element;
	ptr->visited = visit;
	ptr->next = NULL;
	ptr->edges = NULL;

	if(add_to_end)
	{
		curr->next = ptr;
		curr = ptr;
	}
	else
	{
		ptr->next = head;
		head = ptr;
	}
	return ptr;
}


vert* search_vertex(char element, vert **prev)
{
	vert *ptr = head;
	vert *tmp = NULL;
	bool found = false;

	while(ptr != NULL)
	{
		if(ptr->element == element)
		{
			found = true;
			//	printf("found the element [%c]",ptr->element);
			break;
		}
		else
		{
			tmp = ptr;
			ptr = ptr->next;
		}
	}

	if(true == found)
	{
		if(prev)
			*prev = tmp;
		return ptr;
	}
	else
	{
		//	printf("\nelement not found [%c]\n",element);
		return NULL;
	}
}




edgtype* add_edge(char fromelement,char toelement, bool add_to_end)
{
	vert *fromvertex = search_vertex(fromelement,NULL);
	vert *tovertex = search_vertex(toelement,NULL);

	if(fromvertex==NULL || tovertex==NULL){
		//	printf("first insert the vertices [%c] and [%c]",fromelement,toelement);
		return NULL;
	}
	else{
		edgtype *edge=fromvertex->edges;

		if(fromvertex->edges==NULL){

			edgtype *ptr = (edgtype*)malloc(sizeof(edgtype));
			if(NULL == ptr)
			{
				printf("\n Out of memory :add edge\n");
				return NULL;
			}
			ptr->element = toelement;
			ptr->connectsTo = tovertex;
			ptr->next = NULL;

			if(add_to_end)
			{
				fromvertex->edges = ptr;
			}
			else
			{
				//			printf("1.in else in add_edge");
			}
			return ptr;

		}else{

			while(edge->next!=NULL){
				//		printf("traversing edges of vertex [%c] : edge [%c]",fromvertex->element,edge->element);
				edge=edge->next;

			}


			edgtype *ptr = (edgtype*)malloc(sizeof(edgtype));
			if(NULL == ptr)
			{
				printf("\n Out of memory :add edge\n");
				return NULL;
			}
			ptr->element = toelement;
			ptr->connectsTo = tovertex;
			ptr->next = NULL;

			if(add_to_end)
			{
				edge->next = ptr;
			}
			else
			{
				//		printf("2.in else in add_edge");
			}
			return ptr;
		}

	}

}

edgtype* search_edge(char fromelement, char toelement){

	vert *fromvertex = search_vertex(fromelement,NULL);
	vert *tovertex = search_vertex(toelement,NULL);

	if(fromvertex==NULL || tovertex==NULL){
		//	printf("first insert the vertices : [%c] or [%c]",fromelement,toelement);
		return NULL;
	}else{
		edgtype *edge=fromvertex->edges;
		if(fromvertex->edges==NULL){
			return NULL;
		}else{

			while(edge!=NULL){
				if(edge->element==toelement){
					//				printf("found EDGE from [%c] to [%c]",fromelement,edge->element);
					return edge;
				}else{
					edge=edge->next;
				}
			}
			return NULL;

		}


	}

}





static void display(){
	//	cout<<"\n"<<"DISPLAY GRAPH:"<<"\t ";
	vert *temp=head;
	edgtype *temp1;
	while(temp!=NULL){
	//	cout<<"\n"<<temp->element<<"\t ";

		temp1=temp->edges;
		while(temp1!=NULL){

	//		cout<<temp1->element<<" ";
			temp1=temp1->next;
		}
		temp=temp->next;
	}
	//printf("\n");
}


static int node_reach(char dest,vert *startP){
	edgtype *edgeP;

	if (startP->visited)
		return 0;
	if (startP->element == dest)
		return 1;

	startP->visited = 1;
	for (edgeP = startP->edges;
			edgeP != NULL;
			edgeP = edgeP->next) {
		if (node_reach(dest, edgeP->connectsTo))
			return 1;
	}

	return 0;
}


int graph_reach(graph1 graph,char dest,char source){
	vert *vertP;
	vert *startP = NULL;
	for (vertP = head;
			vertP != NULL;
			vertP = vertP->next) {
		vertP->visited = 0;
		if (vertP->element == source)
			startP = vertP;
	}

	if (startP == NULL)
		return 0;
	return node_reach(dest, startP);
}

int query(char e1, char e2){
	if(search_vertex(e1,NULL)==NULL){
		return 1;
	}else if(search_vertex(e2,NULL)==NULL){
		return 2;
	}else if(graph_reach(NULL,e2,e1)>0){
		return 3;
	}else if(graph_reach(NULL,e1,e2)>0){
		return 4;
	}else{
		return 5;
	}
}

void logg(char * log_file,string buf){
	//	printf("******LOG FILE: %s",log_file);
	ofstream outdata;
	outdata.open(log_file,ios::out | ios::app);
	//char logbuf[1024];
	if( !outdata ) { 
		cerr << "Error: file could not be opened111" << endl;
		exit(1);
	}
	outdata << buf << " " << endl;
	outdata.close();

}


void reset(){
	vert *vtemp;
	edgtype * etemp;
	while(head!=NULL){
		vtemp=head;
		etemp=vtemp->edges;
		edgtype* etemp1;
		while(etemp!=NULL){
			etemp1=etemp;
			etemp=etemp->next;
			etemp1=NULL;
		}
		vtemp=NULL;
		head=head->next;
	}
}

int main(int argc,char** argv){
	int ch;
	opterr=0;
	while((ch=getopt(argc,argv,"p:hl:"))!=-1){
		switch(ch){

		case'h':
		{
			printf("\n\tdec_server [−h] [-p port-number] [−l file]");
			printf("\n\n\tOPTIONS :");
			printf("\n\n\t−h : Print a usage summary with all options and exit.");
			printf("\n\t-p : Listen on the given port. If not provided, dec_server will listen on port 9090.");
			printf("\n\t−l : Log all requests and responses to the given file. If not, print all to stdout.");

			printf("\n");

			exit(0);
			break;
		}


		case'l':
		{
			logFile=optarg;
			//		printf("******LOG FILE: %s",logFile);

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
	ofstream out;
	out.open(logFile,ios::out);
	out.close();
	int soc;

	struct sockaddr_in serveradr, remote;
	int newsoc;
	socklen_t sine_size;
	int fdmax;
	fd_set temp_sock;
	soc=socket(AF_INET,SOCK_STREAM,0);

	if(soc==-1){
		cout<<"Could not create a listening socket";
		exit(0);
	}
	memset(&serveradr, 0, sizeof(serveradr));
	serveradr.sin_family      = AF_INET;
	serveradr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveradr.sin_port        = htons(port);
	if ( bind(soc, (struct sockaddr *) &serveradr, sizeof(serveradr)) < 0 )
	{
		cout<<"Could not bind to the listening socket"<<std::endl;
		exit(0);
	}
	cout<<"server started"<<std::endl;
	FD_ZERO(&master);
	listen(soc, 5);
	FD_SET(soc,&master);
	fdmax=soc;
	newsoc = soc;
	std::string log;
	char c='\n';
	while (1)
	{
		temp_sock=master;
		if (select(fdmax+1, &temp_sock, NULL, NULL, NULL) == -1) {
			perror("select");
			exit(4);
		}
		int k=0;
		for(int i=0; i<=fdmax;i++)
		{

			if (FD_ISSET(i, &temp_sock))
			{
				if (i == soc)
				{
					sine_size=sizeof remote;
					newsoc = accept(soc, (struct sockaddr *) &remote, &sine_size);
					cout<<"accepted"<<std::endl;
					if (newsoc==-1)
						continue;
					else
					{
						FD_SET(newsoc, &master);
						if (newsoc> fdmax)
						{
							fdmax = newsoc;
						}
					}
				}
				else
				{
					int rcv=0;
					char buffer1[1000];
					rcv=read(i,buffer1,1000);
					if(rcv<0){
						FD_CLR(i,&master);
						close(i);
					}
					else{
						buffer1[rcv] = '\0';
						char client[1000];

						string argument=buffer1;
						strcpy(client,"\n");
						logg(logFile,buffer1);
						string ibuf[50];
						int events=0;
						string req;
						stringstream istrm(argument);
						while(getline(istrm,req,';')){
							stringstream istream2(req);
							string curr;
							//char* tempcurr;
							string s1,s2;
							char char1,char2;
							int iposition=req.find("insert");
							if(iposition!=-1){
								while(getline(istream2,curr,' ')){
									if(curr.find("->")!=string::npos){
										ibuf[events]=curr;
										//tempcurr=curr;
										s1=curr[0];
										s2=curr[3];
										char1=s1[0];
										char2=s2[0];
										vert *fromvertex = search_vertex(char1,NULL);
										vert *tovertex = search_vertex(char2,NULL);

										if(fromvertex==NULL){

											add_vertex(char1,0,false);
											display();
											add_to_list(char1,'0',1);
										}
										if(tovertex==NULL){

											add_vertex(char2,0,false);
											display();
											add_to_list(char2,'0',1);
										}
										if(graph_reach(NULL,char1,char2)>0){

											soc=1;
										}else{
											if(search_edge(char1,char2)==NULL){
												add_edge(char1,char2,true);
												add_to_list(char1,char2,2);
											}
											else{

											}
											soc=0;
										}

										if(soc==0){
											flag=0;
										}
										else if(soc==1){
											flag=1;
										}
										events++;
									}
								}
								if(flag){
									revoke();
									strcat(client,"\nReply from server : CONFLICT DETECTED . INSERT FAILED  \n");
								}
								else if(!flag){
									strcat(client,"\nReply from server : INSERT DONE \n");
									head_node=NULL;
									curr_node=NULL;
								}

								display();

							}
							else if(req.compare("reset")==0){
								reset();
								char msg[]="\nReply from server : Reset Done\n";
								strcat(client,msg);
							}
							else{
								int qposition=req.find("query");
								if(qposition!=-1){
									char event1,event2;
									stringstream qstream(req);
									event1=req[qposition+6];
									event2=req[qposition+8];
									int q=query(event1,event2);
									int w;
									if(q==5){
										char temp1[2];
										char temp2[2];
										temp1[0]=event1;
										temp1[1]='\0';
										temp2[0]=event2;
										temp2[1]='\0';
										char msg1[]=" and ";
										char msg[]=" are concurrent \n";
										strcat(client,"\n Reply from server :");
										strcat(client,temp1);
										strcat(client, msg1);
										strcat(client,temp2);
										strcat(client,msg);

										//										char msg[]="\nReply from server : Both the events are concurrent \n";
										//										strcat(client,msg);
									}
									else if(q==4){

										char temp1[2];
										char temp2[2];
										temp1[0]=event2;
										temp1[1]='\0';
										temp2[0]=event1;
										temp2[1]='\n';
										strcat(client,"\n Reply from server :");
										strcat(client,temp1);
										char msg[]=" happened before ";
										strcat(client,msg);
										strcat(client,temp2);

										//										char msg[]="\nReply from server :  second event happened before first event \n";
										//										strcat(client,msg);
									}
									else if(q==3){
										char temp1[2];
										char temp2[2];
										temp1[0]=event1;
										temp1[1]='\0';
										temp2[0]=event2;
										temp2[1]='\n';
										strcat(client,"\n Reply from server :");
										strcat(client,temp1);
										char msg[]=" happened before ";
										strcat(client,msg);
										strcat(client,temp2);
										//										char msg[]="\nReply from server :  first event happened before second event \n";
										//										strcat(client,msg);
									}
									else if(q==1 || q==2){
										//char* ev;
										char temp[3];
										char* str;
										if(q==0){
											temp[ 0 ] = event1;
											temp[ 1 ] = '\n';
										}
										else if(q==1){
											temp[ 0 ] = event2;
											temp[ 1 ] = '\n';
										}
										char msg[]= " Event not found :";
										strcat(client,"\n Reply from server :");
										strcat(client,msg);
										strcat(client,temp);
									}


								}
							}
						}
						logg(logFile,client);
						write(i,client,strlen(client));
						printf("%s",client);
					}


				}
			}
		}
	}

	return 0;
}
