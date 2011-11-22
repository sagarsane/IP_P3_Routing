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
#define INF 999999
typedef struct node_{
	double *edge_cost;
	int pred;
	int flag; //denotes whether node's distance to src is permanent or can change
	double cost; // length of node to source
}Node;

Node *node;
FILE *file;
int total_nodes;

void initialize_topology();
void print_topology();
void link_state(int, int, char);
