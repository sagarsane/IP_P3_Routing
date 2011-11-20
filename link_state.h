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
//#include <inf.h>
#define INF 999999
typedef struct node_{
	unsigned int *edge_cost;
	unsigned int pred;
}Node;

Node *node;
