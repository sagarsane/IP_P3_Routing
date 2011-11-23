#ifndef DISTANCE_VECTOR
#define DISTANCE_VECTOR
#include<fcntl.h>

#define INF 99

typedef struct neighbor_ {
	int id;
	double *dv;
} neighbor;

typedef struct node_ {
	int id;
	int no_of_neighbors;
	neighbor *neighbor_list;
	double *dv;
} node;

extern node *nodelist;
extern int total_nodes;

#endif
