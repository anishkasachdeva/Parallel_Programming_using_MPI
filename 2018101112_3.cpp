/* MPI Program Template */
#include <stdio.h>
#include <string.h>
#include "mpi.h"
#include <fstream>
#include <bits/stdc++.h>
using namespace std;
typedef long long int ll;

vector<int> generate_greatest_node_among_neighbours(int edges, int node_beginner_value, int nodes_per_process, int adj_matrix_lineGraph[], int final_coloured_vertices_of_lineGraph[])
{
    int node_value, neighbour_node_value;
    vector<int>greatest_among_neighbours;

   // int uncoloured_vertices = edges;

    for(int i = node_beginner_value; i < node_beginner_value + nodes_per_process; i++)
    {
        if(final_coloured_vertices_of_lineGraph[i] == 0)
        {
            bool flag = true;
            node_value = i;
            for(int j = i+1; j < edges; j++)
            {
                if(final_coloured_vertices_of_lineGraph[j] == 0)
                {
                    neighbour_node_value = j;
                    if(adj_matrix_lineGraph[i*edges + j] == 1)
                    {
                       // cout << "Vertex " << i << " and neighbour " << j << endl;
                        if(neighbour_node_value > node_value)
                        {
                            flag = false;
                            break;
                        }
                    }
               }
            }
            if(flag == true)
            {
                greatest_among_neighbours.push_back(node_value);
                //coloured_vertices_of_lineGraph[node_value] = 1;
                //uncoloured_vertices--;
            }
       }
    }
    return greatest_among_neighbours;
}


void run_parallel(int edges, int numprocs, int rank, int adj_matrix_lineGraph[], int final_coloured_vertices_of_lineGraph[])
{
    int nodes_per_process, last_process_nodes, node_beginner_value;

    nodes_per_process = edges/(numprocs);
    last_process_nodes = nodes_per_process + (edges%(numprocs));
    if(nodes_per_process==0)nodes_per_process=last_process_nodes=1;
    node_beginner_value = ((rank) * nodes_per_process);
    vector<int>greatest_among_neighbours;
    
    if(rank<edges)
    {
        if(rank == edges - 1 || rank == numprocs - 1)
        {
            greatest_among_neighbours = generate_greatest_node_among_neighbours(edges,node_beginner_value, last_process_nodes,adj_matrix_lineGraph,final_coloured_vertices_of_lineGraph); 
        }
        else
        {
            greatest_among_neighbours = generate_greatest_node_among_neighbours(edges,node_beginner_value, nodes_per_process,adj_matrix_lineGraph,final_coloured_vertices_of_lineGraph); 
        }
    }

    for(int i = 0; i < greatest_among_neighbours.size(); i++)
    {
    //    cout << "nodeeeeeeeeeeeee " << greatest_among_neighbours[i] << endl;
        set<int>minimum_color_checker;
        int column;
        for(column = 0; column < edges; column++)
        {
    //        cout << "column " << column << endl;
            if(adj_matrix_lineGraph [greatest_among_neighbours[i] * edges + column ]== 1 && final_coloured_vertices_of_lineGraph[column] > 0)
            {
                minimum_color_checker.insert(final_coloured_vertices_of_lineGraph[column]);
            }
        }

        for(int color_num = 1; ; color_num++)
        {
            if(minimum_color_checker.find(color_num) == minimum_color_checker.end())
            {
                final_coloured_vertices_of_lineGraph[greatest_among_neighbours[i]] = color_num;
                break;
            }
        }
    }



    
    if(rank != 0)
    {
        MPI_Send(final_coloured_vertices_of_lineGraph,edges,MPI_INT,0,0,MPI_COMM_WORLD);
    }
    else
    {

        for(int pid = 1; pid < numprocs; pid++)
        {
            int received_color_array[edges];
            MPI_Recv(received_color_array, edges, MPI_INT, pid, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            
            for(int i = 0; i < edges; i++)
            {
                if(final_coloured_vertices_of_lineGraph[i] != received_color_array[i])
                {
                    final_coloured_vertices_of_lineGraph[i] = max(received_color_array[i], final_coloured_vertices_of_lineGraph[i]);
                }
            }
        }
    }
    MPI_Bcast(final_coloured_vertices_of_lineGraph, edges, MPI_INT, 0, MPI_COMM_WORLD);
}


int main( int argc, char **argv ) {
    int rank, numprocs;

    /* start up MPI */
    MPI_Init( &argc, &argv );

    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &numprocs );
    
    /*synchronize all processes*/
    MPI_Barrier( MPI_COMM_WORLD );
    double tbeg = MPI_Wtime();

    /* write your code here */

    int vertices,edges;
    map<int,pair<int,int> > edge_to_vertex;
    FILE *file = NULL;
    if(rank == 0)
    {
        // cin >> vertices;
        // cin >> edges;
        file = fopen(argv[1], "r");
        // cin>>n>>m;
        fscanf(file, "%d", &vertices);
        fscanf(file, "%d", &edges);
    }


    MPI_Bcast(&edges, 1, MPI_INT, 0, MPI_COMM_WORLD);
    
    if(rank == 0)
    {
        int start_node,end_node;
        int edge_number = 0;
        
        for(int i = 0; i < edges; i++)
        {
            // cin >> start_node >> end_node;
            fscanf(file, "%d", &(start_node));
            fscanf(file, "%d", &(end_node));
            edge_to_vertex[edge_number] = make_pair(start_node-1,end_node-1);
            edge_number++;
        }
        fclose(file);
    }

    int adj_matrix_lineGraph[edges * edges];
    
    for(int i = 0; i < edges; i++)
    {
        for(int j = 0; j < edges; j++)
        {
            adj_matrix_lineGraph[i*edges + j] = 0;
        }
    }
    
    if(rank == 0)
    {
        for(int i = 0; i <  edges; i++)
        {
            for(int j = i+1; j < edges; j++)
            {
                pair<int,int>one = edge_to_vertex[i];
                pair<int,int>two = edge_to_vertex[j];
                
                int v1,v2,v3,v4;
                v1 = one.first;
                v2 = one.second;

                v3 = two.first;
                v4 = two.second;

                if(v1 == v3 || v1 == v4 || v2 == v3 || v2 == v4)
                {
                    adj_matrix_lineGraph[i*edges +j] = 1;
                    adj_matrix_lineGraph[j*edges + i] = 1;
                }
            }
        }
    }

    MPI_Bcast(adj_matrix_lineGraph, sizeof(adj_matrix_lineGraph)/sizeof(int), MPI_INT, 0, MPI_COMM_WORLD);
    
    int final_coloured_vertices_of_lineGraph[edges] ;
    for(int i=0;i<edges;i++)final_coloured_vertices_of_lineGraph[i]=0;

    while(1)
    {
        int uncoloured_vertices = 0;
        for(int i = 0; i < edges; i++)
        {
            if(final_coloured_vertices_of_lineGraph[i] == 0)
            {
                uncoloured_vertices++;
            }
        }
        if(uncoloured_vertices == 0)
        {
            break;
        }
        run_parallel(edges, numprocs, rank, adj_matrix_lineGraph, final_coloured_vertices_of_lineGraph);
    }

    if(rank == 0)
    {
        // for(int i = 0; i < edges; i++)
        // {
        //     cout << final_coloured_vertices_of_lineGraph[i] << " ";
        // }
        int maxi=0;
        for(int i=0;i<edges;i++)if(final_coloured_vertices_of_lineGraph[i]>maxi)maxi=final_coloured_vertices_of_lineGraph[i];
        file = fopen(argv[2], "w");
        fprintf(file, "%d \n", maxi);
        for (int i = 0; i < edges; i++)
        fprintf(file, "%d ", final_coloured_vertices_of_lineGraph[i]);
        fclose(file);
    }
    //cout << endl;
    //}
    

    MPI_Barrier( MPI_COMM_WORLD );
    double elapsedTime = MPI_Wtime() - tbeg;
    double maxTime;
    MPI_Reduce( &elapsedTime, &maxTime, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD );
    if ( rank == 0 ) {
        printf( "Total time (s): %f\n", maxTime );
    }

    /* shut down MPI */
    MPI_Finalize();
    return 0;
}