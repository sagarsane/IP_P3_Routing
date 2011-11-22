#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<math.h>
#include<stdint.h>
#include<string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <pthread.h>
#include<sys/time.h>
#define INF 999999
typedef struct node_{
	double *edge_cost;
	unsigned int pred;
}Node;

Node *node;
FILE *file;
int total_nodes;

void initialize_topology();
void print_topology();
void link_state(int, int);
