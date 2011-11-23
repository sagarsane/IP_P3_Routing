#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"distance_vector.h"

node *nodelist;
int total_nodes;
FILE *file;

void initialize_topology()
{
        int i,j;
        fseek(file,0,SEEK_SET);
        fscanf(file,"%d\n",&total_nodes);

	printf("Total nodes %d\n",total_nodes);

        nodelist = (node *)malloc(sizeof(node)*total_nodes);

        for(i = 0;i< total_nodes;i++)
	{ //initialize nodelist structure
                nodelist[i].id = i;
		nodelist[i].no_of_neighbors = 0;
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

	printf("out of inital setup.. reading file now\n");
        //memset(path,-1,total_nodes*sizeof(int)); //reset path array

        int temp_i = -1,temp_j = -1;
        double temp_cost = 0.0f;
        while(fscanf(file,"%d %d %lf",&temp_i,&temp_j,&temp_cost)!= EOF){
		nodelist[temp_i-1].neighbor_list[nodelist[temp_i-1].no_of_neighbors].id = temp_j-1;
		nodelist[temp_j-1].neighbor_list[nodelist[temp_j-1].no_of_neighbors].id = temp_i-1;

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
		printf("\nFor node %d\n",nodelist[i].id+1);
		printf("No of neighbours %d\n",nodelist[i].no_of_neighbors);
		printf("Neighbors are\n");
		for(j=0;j<nodelist[i].no_of_neighbors;j++)
			printf("%d\t",nodelist[i].neighbor_list[j].id+1);
		printf("\nDistance vector is\n");
		for(j=0;j<total_nodes;j++)
			printf("%.2lf\t",nodelist[i].dv[j]);
	}			
}

int main(int argc, char *argv[]){
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
	printf("File open\n");
	initialize_topology();
	print_topology();
	return 0;
}
