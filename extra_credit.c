#include "extra_credit.h"
int sock;
int total_nodes;
FILE *file;
double *recv_dv;
int recv_id;
struct timespec sleepTime;
struct timespec remainingSleepTime;

int udt_recv()
{
	struct sockaddr_in sender_addr;
        char buf[5000];
	char *a,*b,*c;
        int numbytes,data_length;
        int addr_len = sizeof (sender_addr);
	int i;
        numbytes=recvfrom(sock, buf, 5000 , 0,(struct sockaddr *)&sender_addr, &addr_len);
//	printf("Received BUFFER: %s\n",buf);
	a = strtok(buf," ");
	for(i=0;i<total_nodes;i++)
	{
		if(node.id!=i)
		recv_dv[i] = INF;
		else
		recv_dv[i] = 0.0f;
	}
	//sscanf(buf,"%d ",&recv_id);
	recv_id = atoi(a);
	
	for(i=0;i<total_nodes;i++){
		a = strtok(NULL," ");
		recv_dv[i] = atoi(a);
	}
	
//	printf("Received DV from %d:\n",recv_id+1);
	for(i=0;i<total_nodes;i++){
//		printf("%.2lf ",recv_dv[i]);
	}
//	printf("\n");

}

int udt_send(int i)
{
        struct sockaddr_in their_addr; // connector's address information
        struct hostent *he;
        int numbytes;
        int addr_len;
        int len;
        char buf[5000];
	int j;
        //HIDEprintf("In udt_send segment: %d and receiver %d\n",seg_index,server_index);
        if ((he=gethostbyname(node.neighbor_list[i].ipaddr)) == NULL) {  // get the host info
                printf("\nHost not found %s\n",node.neighbor_list[i].ipaddr);
                exit(-1);
        }

        their_addr.sin_family = AF_INET;     // host byte order
        their_addr.sin_port = htons(node.neighbor_list[i].portnum);
        their_addr.sin_addr = *((struct in_addr *)he->h_addr);
	memset(buf,0,5000);
	
	char tmp_buf[1000];
	sprintf(buf,"%d ",node.id);
	for(j=0;j<total_nodes;j++){
		sprintf(tmp_buf,"%.2lf ",node.dv[j]);
		strcat(buf,tmp_buf);
	}
        len = strlen(buf);
        if (sendto(sock,buf, len, 0, (struct sockaddr *)&their_addr, sizeof (their_addr)) == -1) {
               printf("Error in sending");
                exit(-1);
        }
 //       fflush(stdout);
//	printf("Will send: %s\n",buf);
}




void populate_public_ip()
{

        struct ifaddrs *myaddrs, *ifa;
        void *in_addr;
        char buf[64], intf[128];

        strcpy(node.iface_name, "");

        if(getifaddrs(&myaddrs) != 0) {
                printf("getifaddrs failed! \n");
                exit(-1);
        }

        for (ifa = myaddrs; ifa != NULL; ifa = ifa->ifa_next) {

                if (ifa->ifa_addr == NULL)
                        continue;

                if (!(ifa->ifa_flags & IFF_UP))
                        continue;

                switch (ifa->ifa_addr->sa_family) {

                        case AF_INET: {
                                struct sockaddr_in *s4 = (struct sockaddr_in *)ifa->ifa_addr;
                                in_addr = &s4->sin_addr;
                                break;
                        }

                        case AF_INET6: {
                                struct sockaddr_in6 *s6 = (struct sockaddr_in6 *)ifa->ifa_addr;
                                in_addr = &s6->sin6_addr;
                                break;
                        }
                                                      
                       default:
                                continue;
                }

                if (inet_ntop(ifa->ifa_addr->sa_family, in_addr, buf, sizeof(buf))) {
                        if ( ifa->ifa_addr->sa_family == AF_INET && strcmp(ifa->ifa_name, "lo")!=0 ) {
                                printf("\nDo you want the sender to bind to %s interface?(y/n): ", ifa->ifa_name);
                                scanf("%s", intf);
                                if ( strcmp(intf, "n") == 0 )
                                        continue;
                                sprintf(node.ipaddr, "%s", buf);
                                sprintf(node.iface_name, "%s", ifa->ifa_name);

                        }
                }
        }

        freeifaddrs(myaddrs);

        if ( strcmp(node.iface_name, "") == 0 ) {
                printf("Either no Interface is up or you did not select any interface ..... \nclient Exiting .... \n\n");
                exit(0);
        }

         printf("\n\nMy public interface and IP is:  %s %s\n\n", node.iface_name, node.ipaddr);

}


void print_dv(){
	int i;

	for(i = 0; i <total_nodes ; i++){
		printf("%.2lf  ",node.dv[i]);		
	}
	printf("\n");
	for( i = 0;i < node.no_of_neighbors; i++){
		printf("%s\t%d\n",node.neighbor_list[i].ipaddr,node.neighbor_list[i].portnum);
	}
}

void initialize(int argc, char *argv[]){
	int i;
	total_nodes = atoi(argv[3]);
	node.portnum = atoi(argv[2]);
	node.no_of_neighbors = atoi(argv[4]);
	populate_public_ip();


	recv_dv = (double *)malloc(sizeof(double) * total_nodes);
	node.dv = (double *)malloc(sizeof(double) * total_nodes);
	node.next_hop = (int *)malloc(sizeof(int) * total_nodes);
	node.neighbor_list = (neighbor *)malloc(sizeof(neighbor) * node.no_of_neighbors);
	node.id = atoi(argv[1])-1;
	node.send_flag = 1;
	for(i = 0;i < node.no_of_neighbors; i++){
		node.neighbor_list[i].dv = (double *)malloc(sizeof(double) * total_nodes);
	}

	
	file = fopen(argv[5],"r");
	if(!file){
		perror("Cannot open FIle\n");
		exit(-1);
	}

	
	for(i = 0; i < total_nodes; i++){
		if(node.id != i){
			node.next_hop[i] = -1;
			node.dv[i] = INF;
		}
		else{
			node.dv[i] = 0.0f;
			node.next_hop[i] = i;
		}

		
	}
	
	int temp_id,temp_portnum;
	double temp_cost;
	char temp_ipaddr[128];
	i = 0;
	while(fscanf(file,"%d %s %d %lf",&temp_id,temp_ipaddr,&temp_portnum,&temp_cost)!=EOF){
		node.neighbor_list[i].id = temp_id-1;
		node.dv[temp_id-1] = temp_cost;
		strcpy(node.neighbor_list[i].ipaddr,temp_ipaddr);
		node.neighbor_list[i].portnum = temp_portnum;
		i++;
	}

 	struct sockaddr_in my_addr; 
        if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
                printf("Error creating socket\n");
                exit(-1);
        }

        my_addr.sin_family = AF_INET;         // host byte order
        my_addr.sin_port = htons(node.portnum);// short, network byte order
        my_addr.sin_addr.s_addr = INADDR_ANY; // automatically fill with my IP
        memset(my_addr.sin_zero, '\0', sizeof my_addr.sin_zero);

        if (bind(sock, (struct sockaddr *)&my_addr, sizeof my_addr) == -1) {
                printf("\n bind failed");
                exit(-1);
        }

	
}

void update_distance_vector(int node_id,int neighbor_index)
{
        int i;
        int flag = 0;
//        int neighbor_id = nodelist[node_id].neighbor_list[neighbor_index].id;
	int neighbor_id = neighbor_index;
        //printf("node is %d and neighbor is %d\n",node_id,neighbor_id);
        for(i=0;i<total_nodes;i++)
        {
                double new_distance = node.dv[neighbor_id]+recv_dv[i];//node.neighbor_list[neighbor_id].dv[i];//nodelist[node_id].dv[neighbor_id]+nodelist[node_id].neighbor_list[neighbor_index].dv[i];
//              printf("for node %d current distance is %.2lf and new distance is %.2lf\n",i,nodelist[node_id].dv[i],new_distance);
                if(node.dv[i]>new_distance)
                {
			printf("\nSetting next hop of %d to %d\n\n",i,neighbor_id);
                        node.next_hop[i] = neighbor_id;
                        node.dv[i] = new_distance;
                        node.send_flag++;
                        flag++;
                }
        }
//        if(flag)
 //       {
                //nodelist[node_id].count++;
		
                //printf("send flag is %d\n",nodelist[node_id].send_flag);
  //      }
}

void print_r_table()
{
        int j;
        printf("Destination\tNext Hop\tCost\n");
        printf("---------------------------------------------------------------------------\n");
        for(j=0;j<total_nodes;j++)
                printf("    %d\t\t  %d\t\t%.2lf\n",j+1,node.next_hop[j]+1,node.dv[j]);
}

void * print_table()
{
	
	while(1)
	{
		nanosleep(&sleepTime,&remainingSleepTime);
		print_r_table();
	}
}

int main(int argc, char *argv[]){

	int i;
	sleepTime.tv_sec=5;
        sleepTime.tv_nsec=0;

	pthread_t printer;
	pthread_create(&printer,NULL,print_table,NULL);
	initialize(argc,argv);
	print_dv();
	for(i=0;i<node.no_of_neighbors;i++)
		udt_send(i);
	while(1){
		udt_recv();
		update_distance_vector(node.id,recv_id);
		if(node.send_flag)
		{
			for(i=0;i<node.no_of_neighbors;i++)
				udt_send(i);
			//node.send_flag = 0;
		}
		node.send_flag = 0;
	}
}
