#include "link_state.h"

double getcputime(void)        
{ 
	struct timeval tim;        
        struct rusage ru;        
        getrusage(RUSAGE_SELF, &ru);        
        tim=ru.ru_utime;        
        double t=(double)tim.tv_sec + (double)tim.tv_usec / 1000000.0;
	printf("T: %lf\n",t); 
        tim=ru.ru_stime;        
        t+=(double)tim.tv_sec + (double)tim.tv_usec / 1000000.0;        
	printf("T+: %lf\n",t);
        return t; 
}

void save_path(int current_node){
	int i;
	int j = 0,k;
	for(i=0;i<total_nodes;i++)
	{
		//printf("Pred for %d is: %d\n",i+1,node[i].pred+1);
		if(i!=current_node - 1){
			//node[current_node - 1].saved_cost
			k = i;
			j = node[i].pred;
		
			while(j!=current_node-1){
				k = j;
				j = node[j].pred;
			}	
			node[current_node - 1].next_hop[i] = k;
			node[current_node - 1].saved_cost[i] = node[i].cost;	
		}
		else{
			node[current_node - 1].next_hop[i] = i;
			node[current_node - 1].saved_cost[i] = 0.0f;
		}	
	}
	
}

void find_path(int node1, int node2)
{
}

void reset_topology(){
	int i;
	for(i=0;i<total_nodes;i++){
		node[i].flag = 0;
		node[i].pred = i;
	}
	
}
int main(int argc, char *argv[]){
	int i;
	//clock_t start,end;
	long cpu_time_used;
	struct tms *buf,*buf1;
	struct timeval start, end;
	total_nodes = -1;
	
	if(argc != 4){
		perror("Incorrect command line arguments\n");
		exit(-1);
	}
	file = fopen(argv[1],"r");
	initialize_topology();
	//start = clock();
	//start = times(buf);

	//s = getcputime();
	//printf("Start : %ld\n",(long)start);
	for(i=0;i<total_nodes;i++)
	{
		gettimeofday(&start,NULL);
		link_state(i+1,0);
		save_path(i+1);
	//	print_topology(i+1);
		reset_topology();
		gettimeofday(&end, NULL);
		printf("Time taken for source Node %d is : %.15lf microseconds\n",i+1,(double)(end.tv_usec-start.tv_usec + (end.tv_sec-start.tv_sec)));


	}

	//end = clock();
	//end = times(&buf1);
	//printf("End : %ld\n",(long)end);
	//cpu_time_used = ((long) (end - start)) / CLOCKS_PER_SEC;
	//printf("Time taken : %ld\n",cpu_time_used);

	print_topology(atoi(argv[2]));
	print_topology(atoi(argv[3]));
	printf("Cost of the Least cost path between Node1 (%d) and Node2(%d) is : %lf\n",atoi(argv[2]),atoi(argv[3]),node[atoi(argv[2])-1].saved_cost[atoi(argv[3])-1]);
	
	for(i=0;i<total_nodes;i++)
	{
		free(node[i].edge_cost);
		free(node[i].saved_cost);
		free(node[i].next_hop);
	}
	free(node);
	/*//struct tms buf1;
	end = times(&buf1);
	printf("End : %.8lf\n",(double)end);
	cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
	initialize_topology();
	link_state(atoi(argv[3]),atoi(argv[2]),1);
	printf("Time taken : %lf\n",cpu_time_used);
	*/
}	

void initialize_topology(){
	int i,j;
	if(!file){
		perror("Cannot Open File\n");
		exit(-1);
	}
	fseek(file,0,SEEK_SET);	
	fscanf(file,"%d\n",&total_nodes);
//	printf("%d\n",total_nodes);

	node = (Node *)malloc(sizeof(Node)*total_nodes);

	for(i = 0;i< total_nodes;i++){ //initialize node structure
		node[i].edge_cost = (double *)malloc(sizeof(double) * total_nodes);
		node[i].next_hop = (int *)malloc(sizeof(int) * total_nodes);
		node[i].saved_cost = (double *)malloc(sizeof(double) * total_nodes);
		for(j = 0;j < total_nodes;j++)
			node[i].edge_cost[j] = INF;	
		node[i].pred = i;
		node[i].flag=0; //not yet permanent
	//	node[i].cost=INF;//start all nodes with infinite cost
	}
	
	//memset(path,-1,total_nodes*sizeof(int)); //reset path array

	int temp_i = -1,temp_j = -1;
	double temp_cost = 0.0f;
	while(fscanf(file,"%d %d %lf",&temp_i,&temp_j,&temp_cost)!= EOF){
		node[temp_i-1].edge_cost[temp_j-1] = temp_cost;
		node[temp_j-1].edge_cost[temp_i-1] = temp_cost;
	}
}


void initialization()
{
	int k = 0;
	int i;	
	for(i=0;i<total_nodes;i++)
        {
                if(i!=k)
                {
                        if(node[k].edge_cost[i]!=INF){
                                //printf("D[%d] = C(%d,%d) = %lf\n",i+1,k+1,i+1,node[k].edge_cost[i]);
                                node[i].cost = node[k].edge_cost[i]; //initialize D(v) = c(u,v)
                        }
                        else{
                                //printf("D[%d] = INFININTY\n",i+1);
                                node[i].cost = INF;
                        }
                }
                else{
                        node[i].cost = 0.0f;
                        //printf("D[%d] = 0\n",i+1);
                }
        }


}
void print_topology(int src)
{
	int k;
	/*
	for(i = 0;i < total_nodes; i++){
		printf("%d : ",i+1);
		for(j = 0;j < total_nodes; j++){
			if(node[i].edge_cost[j]!=0)
			printf("\t%d -> %.2lf",j+1, node[i].edge_cost[j]);
		}
		printf("\n");
	}
	*/
	printf("\nRouting table of source node %d -->\n\n",src);
        printf("  Destination\t  Next Hop \tCost to Destination\n");
        printf("---------------------------------------------------\n");

        for(k = 0;k < total_nodes; k++){
                       // if(node[i].edge_cost[k]!=0)
                        //if(node[k].cost!=999999)
                        printf("  %d\t\t  %d   \t\t\t   %.2lf\n",k+1,node[src-1].next_hop[k]+1,node[src-1].saved_cost[k]);
                        //else
                        //printf("  %d\t  %d\tINFINITY\n",src,k+1);
        }

}

int all_flags_set()
{
	int i;
	for(i=0;i<total_nodes;i++)
	{	
		if(node[i].flag==0)
			return 0;
	}
	return 1;
}

void link_state(int src, char flag){
	
	int i,k;
	double min;

	k=src-1;
        //node[src-1].cost=0.0f;
        node[k].flag=1;

	for(i=0;i<total_nodes;i++)
	{
		if(i!=k)
		{
			if(node[k].edge_cost[i]!=INF){
				//printf("D[%d] = C(%d,%d) = %lf\n",i+1,k+1,i+1,node[k].edge_cost[i]);
				node[i].cost = node[k].edge_cost[i]; //initialize D(v) = c(u,v)
				node[i].pred = k;
			}
			else{
				//printf("D[%d] = INFININTY\n",i+1);
				node[i].cost = INF;
			}
		}
		else{
			node[i].cost = 0.0f;
			//printf("D[%d] = 0\n",i+1);
		}
	}

        do
        {
	        min=INF;
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
                for(i=0;i<total_nodes;i++)
                {
		
                        if(node[k].edge_cost[i]!=INF && node[i].flag == 0)
                        {
                                if(node[k].cost + node[k].edge_cost[i] < node[i].cost)  //update predecessor and cost to src node
				{
					//printf("Setting Pred for %d to %d\n", i+1,k+1);
                                        node[i].pred = k;
                                        node[i].cost = node[k].cost+node[k].edge_cost[i];
                                }
                        }
			

                }

        }while(!all_flags_set());
	
}

