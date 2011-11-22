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

	for(i = 0;i< total_nodes;i++){ //initialize node structure
		node[i].edge_cost = (double *)malloc(sizeof(double) * total_nodes);
		memset(node[i].edge_cost,0.0,sizeof(double)*total_nodes);
		node[i].pred = i;
		node[i].flag=0; //not yet permanent
		node[i].cost=INF;//start all nodes with infinite cost
	}
	
	//memset(path,-1,total_nodes*sizeof(int)); //reset path array

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

void link_state(int src, int dest){
	
	int i,k;
	double min;

	k=dest-1;
	node[dest-1].cost=0.0f;
	node[dest-1].flag=1;

	do	
	{
		for(i=0;i<total_nodes;i++)
		{
			if(node[k].edge_cost[i]!=0 && node[i].flag == 0)
			{
				if(node[k].cost + node[k].edge_cost[i] < node[i].cost)	//update predecessor and cost to src node
				{
					node[i].pred = k;			
					node[i].cost = node[k].cost+node[k].edge_cost[i];
				}
			}
			
		}
		k=0,min=INF;
		//Find next min
		for(i=0;i<total_nodes;i++)
		{
			if(node[i].flag ==0 && node[i].cost < min)
			{
				min = node[i].cost;
				k=i;
			}
		}
		node[k].flag=1;
	}while(k!=(src-1));

	printf("\nLeast cost from source to destination is : %lf\n",node[src-1].cost);

	printf("\nRouting table of source node :\n");
	i=src-1;
        printf("%d : ",i+1);
        for(k = 0;k < total_nodes; k++){
                        if(node[i].edge_cost[k]!=0)
                        printf("%d -> %lf\t",k+1, node[i].edge_cost[k]);
        }
        
	printf("\n\nRouting table of destination node :\n");
        i=dest-1;
        printf("%d : ",i+1);
        for(k = 0;k < total_nodes; k++){
                        if(node[i].edge_cost[k]!=0)
                        printf("%d -> %lf\t",k+1, node[i].edge_cost[k]);
        }
	
	printf("\n\n");
}

