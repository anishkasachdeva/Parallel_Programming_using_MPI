/* MPI Program Template */
#include <stdio.h>
#include <string.h>
#include "mpi.h"
#include <fstream>
#include <bits/stdc++.h>
using namespace std;
typedef long long int ll;

// void assign_minimum_color_scheme(int edges)
// {
//     for(int i = 0; i < edges; i++)
//     {
//         for(int j = 0; j < edges; j++)
//         {

//         }
//     }
// }


set<int> generate_greatest_node_among_neighbours(int edges, int node_beginner_value, int nodes_per_process, vector<vector<int> > adj_matrix_lineGraph)
{
    int node_value, neighbour_node_value;
    set<int>greatest_among_neighbours;
    for(int i = node_beginner_value; i < node_beginner_value + nodes_per_process; i++)
    {
        bool flag = true;
        node_value = i;
        for(int j = 0; j < edges; j++)
        {
            neighbour_node_value = j;
            if(adj_matrix_lineGraph[i][j] == 1)
            {
                if(neighbour_node_value > node_value)
                {
                    flag = false;
                    break;
                }
            }
        }
        if(flag == true)
        {
            greatest_among_neighbours.insert(node_value);
            //whatever data structure we want to use
        }
    }
    return greatest_among_neighbours; 
}


    
void run_parallel(int numprocs, int rank, int vertices,int edges, vector<vector<int> > adj_matrix_lineGraph)
{
    cout << "Came Here" << rank << endl << endl;
    int nodes_per_process = vertices/numprocs;
    int last_process_nodes = nodes_per_process + (vertices%numprocs);
    
    int node_beginner_value;
    
    //for(int pid = 1; pid < numprocs; pid++)
    //{
        set<int>greatest_among_neighbours;
        node_beginner_value = (rank * nodes_per_process);

        cout << "node_beginner_value " <<  node_beginner_value << endl;
        if(rank == numprocs - 1)
        {
            greatest_among_neighbours = generate_greatest_node_among_neighbours(edges,node_beginner_value, last_process_nodes,adj_matrix_lineGraph); 
        }
        else
        {
            greatest_among_neighbours = generate_greatest_node_among_neighbours(edges,node_beginner_value, nodes_per_process,adj_matrix_lineGraph);
            //assign_minimum_color_scheme();
        }
    //}
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
    map<pair<int, int>, int> vertex_to_edge;
    map<int,pair<int,int>> edge_to_vertex;
    
    if(rank == 0)
    {
        //freopen(argv[1], "r", stdin);
        //freopen(argv[2], "a", stdout);

        cin >> vertices;
        cin >> edges;
    }


    MPI_Bcast(&edges, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&vertices, 1, MPI_INT, 0, MPI_COMM_WORLD);
    
    if(rank == 0)
    {
        vector<vector<int>> adj_matrix_originalGraph(vertices, vector<int>(vertices, 0));
        
        int start_node,end_node;
        int edge_number = 0;
        
        for(int i = 0; i < edges; i++)
        {
            cin >> start_node >> end_node;


            vertex_to_edge[{start_node-1,end_node-1}] = edge_number;
            edge_to_vertex[edge_number] = make_pair(start_node-1,end_node-1);

            //adj_matrix_originalGraph[start_node-1][end_node-1] = 1;
            //adj_matrix_originalGraph[end_node-1][start_node-1] = 1;

            edge_number++;
        }
        // map<int,pair<int,int>>::iterator itr;

        // for(itr = edge_to_vertex.begin(); itr != edge_to_vertex.end(); itr++)
        // {
        //     cout << itr->first << " - " << itr->second.first << " , " << itr->second.second << endl;
        // }

        // cout << "original" << endl << endl;
        // for(int i = 0; i < vertices; i++)
        // {
        //     for(int j = 0; j < vertices; j++)
        //     {
        //         cout << adj_matrix_originalGraph[i][j] << " ";
        //     }
        //     cout << endl;
        // }
        // cout << endl;

    }

    vector<vector<int> > adj_matrix_lineGraph(edges , vector<int> (edges, 0));
        
    if(rank == 0)
    {
        for(int i = 0; i <  edges; i++)
        {
            for(int j = i+1; j < edges; j++)
            {
                //if(i != j)
                //{
                    if(edge_to_vertex.find(i) != edge_to_vertex.end() && edge_to_vertex.find(j) != edge_to_vertex.end())
                    {
                        //cout << "i is " << i << " and j is " << j << endl;
                        pair<int,int>one = edge_to_vertex[i];
                        pair<int,int>two = edge_to_vertex[j];
                        
                        int v1,v2,v3,v4;
                        v1 = one.first;
                        v2 = one.second;

                        v3 = two.first;
                        v4 = two.second;

                        // cout << "first pair" << " ";
                        // cout << v1 << " " << v2 << endl;

                        // cout << "second pair" << " ";
                        // cout << v2 << " " << v4 << endl << endl;

                        if(v1 == v3 || v1 == v4 || v2 == v3 || v2 == v4)
                        {
                            adj_matrix_lineGraph[i][j] = 1;
                            adj_matrix_lineGraph[j][i] = 1;
                        }
                    }
                //}
            }
        }
        cout << endl;
        cout << "Line Master Process" << endl << endl;
        for(int i = 0; i < edges; i++)
        {
            for(int j = 0; j < edges; j++)
            {
                cout << adj_matrix_lineGraph[i][j] << " ";
            }
            cout << endl;
        }
        cout << endl;
    }

    for (int i = 0; i < edges; i++)
        MPI_Bcast(&adj_matrix_lineGraph[i][0], edges*edges, MPI_INT, 0, MPI_COMM_WORLD);
    
    if(rank != 0)
    {
        //cout << edges << endl;
        //cout << adj_matrix_lineGraph[0][1] << endl;
        //cout << endl;
        //cout << "Line" << endl << endl;
        for(int i = 0; i < edges; i++)
        {
            for(int j = 0; j < edges; j++)
            {
                cout << adj_matrix_lineGraph[i][j] << " ";
            }
            cout << endl;
        }
        cout << endl;
        
        run_parallel(numprocs,rank,vertices,edges,adj_matrix_lineGraph);
        
    }

    
    // else
    // {

    // }
    
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