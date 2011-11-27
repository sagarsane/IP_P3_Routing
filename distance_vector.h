#ifndef DISTANCE_VECTOR
#define DISTANCE_VECTOR
#include<fcntl.h>
#define INF 99

typedef struct neighbor_ {
	int id;
	int index_in_neighbor;
	double *dv;
} neighbor;

typedef struct node_ {
	int id;
	int send_flag;
	int count;
	int no_of_neighbors;
	neighbor *neighbor_list;
	double *dv;
	int *next_hop;
} node;

typedef struct queue_node_ {
	int node_id;
	int neighbor_id;
} queue_node;

typedef struct queue_ {
	int index;
	queue_node *id_list;
} queue;

extern node *nodelist;
extern int total_nodes;

#endif
