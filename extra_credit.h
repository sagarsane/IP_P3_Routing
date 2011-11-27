#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<math.h>
#include<stdint.h>
#include<string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <pthread.h>
#include <time.h>
#define INF 999999.0

typedef struct neighbor_ {
        int id;
	char ipaddr[16];
	int portnum;
        //int index_in_neighbor;
        double *dv;
} neighbor;


typedef struct node_ {
        int id;
    	char iface_name[64];
	char ipaddr[128];
	int portnum;
        int send_flag;
        //int count;
        int no_of_neighbors;
        neighbor *neighbor_list;
        double *dv;
        int *next_hop;
} Node;


Node node;

