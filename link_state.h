#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<math.h>
#include<stdint.h>
#include<string.h>
#include<time.h>
#include<sys/time.h>
#include<sys/resource.h>
#define INF 999999.0
typedef struct node_{
	double *edge_cost;
	int pred;
	int flag; //denotes whether node's distance to src is permanent or can change
	double cost; // length of node to source
	
	double *saved_cost;
	int *next_hop;
}Node;

Node *node;
FILE *file;
int total_nodes;

void initialize_topology();
void print_topology(int);
void link_state(int, char);
void initialization();
