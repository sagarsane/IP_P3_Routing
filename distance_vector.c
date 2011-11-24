#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"distance_vector.h"

node *nodelist;
int total_nodes;
FILE *file;

queue node_queue;
void init_queue()
{
	int i;
	node_queue.index = 0;
	node_queue.id_list = (queue_node*)malloc(sizeof(queue_node)*1000000);
	for(i=0;i<total_nodes;i++)
		node_queue.id_list[i].node_id=-1;
}
void add_to_queue(int id,int neighbor)
{
	node_queue.id_list[node_queue.index].node_id = id;
	node_queue.id_list[node_queue.index].neighbor_id = neighbor;
	node_queue.index++;
}
queue_node remove_from_queue()
{
	int i;
	queue_node ret = node_queue.id_list[0];
	printf("Index is %d\n",node_queue.index);
	if(node_queue.index>=0)
	{
		for(i=1;i<node_queue.index;i++)
			node_queue.id_list[i-1] = node_queue.id_list[i];	
		node_queue.id_list[node_queue.index].node_id=-1;
		node_queue.index--;
	}
	return ret;
}

void initialize_topology()
{
        int i,j;
        fseek(file,0,SEEK_SET);
        fscanf(file,"%d\n",&total_nodes);

//	printf("Total nodes %d\n",total_nodes);

        nodelist = (node *)malloc(sizeof(node)*total_nodes);

        for(i = 0;i< total_nodes;i++)
	{ //initialize nodelist structure
                nodelist[i].id = i;
		nodelist[i].send_flag = 1;
		nodelist[i].no_of_neighbors = 0;
		nodelist[i].count = 0;
		nodelist[i].dv = (double*) malloc(sizeof(double)*total_nodes);
		for(j=0;j<total_nodes;j++)
		{
			if(i==j)
				nodelist[i].dv[j] = 0;
			else
				nodelist[i].dv[j] = INF;
		}
		nodelist[i].neighbor_list = (neighbor*)malloc(sizeof(neighbor)*total_nodes);
		memset(nodelist[i].neighbor_list,0,sizeof(neighbor)*total_nodes);
        }

//	printf("out of inital setup.. reading file now\n");
        //memset(path,-1,total_nodes*sizeof(int)); //reset path array

        int temp_i = -1,temp_j = -1;
        double temp_cost = 0.0f;
        while(fscanf(file,"%d %d %lf",&temp_i,&temp_j,&temp_cost)!= EOF){
		nodelist[temp_i-1].neighbor_list[nodelist[temp_i-1].no_of_neighbors].id = temp_j-1;
		nodelist[temp_j-1].neighbor_list[nodelist[temp_j-1].no_of_neighbors].id = temp_i-1;

		nodelist[temp_i-1].neighbor_list[nodelist[temp_i-1].no_of_neighbors].index_in_neighbor = nodelist[temp_j-1].no_of_neighbors;
		nodelist[temp_j-1].neighbor_list[nodelist[temp_j-1].no_of_neighbors].index_in_neighbor = nodelist[temp_i-1].no_of_neighbors;

		nodelist[temp_i-1].neighbor_list[nodelist[temp_i-1].no_of_neighbors].dv = (double*)malloc(sizeof(double)*total_nodes);
		for(i=0;i<total_nodes;i++)
			nodelist[temp_i-1].neighbor_list[nodelist[temp_i-1].no_of_neighbors].dv[i] = INF;
		nodelist[temp_j-1].neighbor_list[nodelist[temp_j-1].no_of_neighbors].dv = (double*)malloc(sizeof(double)*total_nodes);
		for(i=0;i<total_nodes;i++)
			nodelist[temp_j-1].neighbor_list[nodelist[temp_j-1].no_of_neighbors].dv[i] = INF;

		nodelist[temp_i-1].no_of_neighbors++;
		nodelist[temp_j-1].no_of_neighbors++;

		nodelist[temp_i-1].dv[temp_j-1] = temp_cost;
		nodelist[temp_j-1].dv[temp_i-1] = temp_cost;
        }
}

void print_topology()
{
	int i,j;
	for(i=0;i<total_nodes;i++)
	{
		printf("\nNode %d ",nodelist[i].id+1);
		printf("Neighbours %d\n",nodelist[i].no_of_neighbors);
		printf("Count is: %d\n",nodelist[i].count);
//		printf("Neighbors are\n");
		for(j=0;j<nodelist[i].no_of_neighbors;j++)
			printf("%2d\t",nodelist[i].neighbor_list[j].id+1);
		printf("\n");
		for(j=0;j<nodelist[i].no_of_neighbors;j++)
			printf("%2d\t",nodelist[i].neighbor_list[j].index_in_neighbor);
		printf("\n");
		printf("\nDistance vector is\n");
		for(j=0;j<total_nodes;j++)
			printf("%.2lf\t",nodelist[i].dv[j]);
		printf("\n");
	}			
}

void send_distance_vector(int node_id)
{
	int i,j;
	for(i=0;i<nodelist[node_id].no_of_neighbors;i++)
	{
		int curr_neighbor = nodelist[node_id].neighbor_list[i].id;
		int curr_index = nodelist[node_id].neighbor_list[i].index_in_neighbor;
		for(j=0;j<total_nodes;j++)
			nodelist[curr_neighbor].neighbor_list[curr_index].dv[j] = nodelist[node_id].dv[j];
		add_to_queue(curr_neighbor,curr_index);
	}
}

void update_distance_vector(int node_id,int neighbor_index)
{
	int i;
	int flag = 0;
	int neighbor_id = nodelist[node_id].neighbor_list[neighbor_index].id;
	printf("node is %d and neighbor is %d\n",node_id,neighbor_id);
	for(i=0;i<total_nodes;i++)
	{	
		double new_distance = nodelist[node_id].dv[neighbor_id]+nodelist[node_id].neighbor_list[neighbor_index].dv[i];
		printf("for node %d current distance is %.2lf and new distance is %.2lf\n",i,nodelist[node_id].dv[i],new_distance);
		if(nodelist[node_id].dv[i]>new_distance)
		{
			nodelist[node_id].dv[i] = new_distance;
			nodelist[node_id].send_flag++;
			flag++;
		}	
	}
	if(flag)
	{
		nodelist[node_id].count++;
		printf("send flag is %d\n",nodelist[node_id].send_flag);
	}
}

void print_dv(int i)
{
	int j;
	 for(j=0;j<total_nodes;j++)
		printf("%.2lf\t",nodelist[i].dv[j]);
}

int get_max_count()
{
	int i;
	int max_count_id = 0;
	for(i=1;i<total_nodes;i++)
		if(nodelist[max_count_id].count<nodelist[i].count)
			max_count_id = i;
	return max_count_id;
}

int main(int argc, char *argv[]){
	int initial_node;
	queue_node curr_node;
	total_nodes = -1;
        if(argc != 5){
                perror("Incorrect command line arguments\n");
                exit(-1);
        }
        file = fopen(argv[2],"r");
        if(!file){
                perror("Cannot Open File\n");
                exit(-1);
        }
//	printf("File open\n");
	initialize_topology();
	init_queue();
	printf("Before algorithm\n");
	print_topology();
	initial_node = atoi(argv[1])-1;
	send_distance_vector(initial_node);
	while(1)
	{
		curr_node = remove_from_queue();
		printf("inside while %d\n",curr_node.node_id);
		if(curr_node.node_id == -1)
			break;
		//nodelist[curr_node.node_id].send_flag = 0;

		update_distance_vector(curr_node.node_id,curr_node.neighbor_id);
	//	printf("inside while again %d\n",curr_node.node_id);
		if(nodelist[curr_node.node_id].send_flag)
			send_distance_vector(curr_node.node_id);
	//	printf("inside while 3rd time %d\n",curr_node.node_id);
		nodelist[curr_node.node_id].send_flag = 0;
	//	printf("inside while finally %d\n",curr_node.node_id);
	}
	printf("After Algorithm\n");
	print_topology();

	printf("\n\nDistance vector for node %d is\n",atoi(argv[3]));
	print_dv(atoi(argv[3])-1);

	printf("\nDistance vector for node %d is\n",atoi(argv[4]));
	print_dv(atoi(argv[4])-1);

	int max_count_id = get_max_count();
	printf("\nMax iterations is %d for node %d \n",nodelist[max_count_id].count,max_count_id+1);
	
	return 0;
}
