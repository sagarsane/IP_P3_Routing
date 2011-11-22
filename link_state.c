#include "link_state.h"

int main(int argc, char *argv[]){
	total_nodes = -1;
	if(argc != 4){
		perror("Incorrect command line arguments\n");
		exit(-1);
	}
	file = fopen(argv[1],"r");
	initialize_topology();
	print_topology();
	link_state(atoi(argv[2]),atoi(argv[3]));
}	

void initialize_topology(){
	int i,j;
	if(!file){
		perror("Cannot Open File\n");
		exit(-1);
	}
	
	fscanf(file,"%d\n",&total_nodes);
	printf("%d\n",total_nodes);

	node = (Node *)malloc(sizeof(Node)*total_nodes);

	for(i = 0;i< total_nodes;i++){
		node[i].edge_cost = (double *)malloc(sizeof(double) * total_nodes);
		memset(node[i].edge_cost,0.0,sizeof(double)*total_nodes);
		node[i].pred = i;
	}
	int temp_i = -1,temp_j = -1;
	double temp_cost = 0.0f;
	while(fscanf(file,"%d %d %lf",&temp_i,&temp_j,&temp_cost)!= EOF){
		node[temp_i-1].edge_cost[temp_j-1] = temp_cost;
		node[temp_j-1].edge_cost[temp_i-1] = temp_cost;
	}
}

void print_topology()
{
	int i,j;
	for(i = 0;i < total_nodes; i++){
		printf("%d : ",i+1);
		for(j = 0;j < total_nodes; j++){
			if(node[i].edge_cost[j]!=0)
			printf("\t%d -> %lf",j+1, node[i].edge_cost[j]);
		}
		printf("\n");
	}
	
}

void link_state(int source, int destination){
}

