/* MPI Program Template */
#include <stdio.h>
#include <string.h>
#include "mpi.h"
#include <fstream>
#include <bits/stdc++.h>
using namespace std;
typedef long long int ll;


void printResult(double sum)
{
    printf("%.6f\n", sum);
}


int main( int argc, char **argv ) {
    
    int rank, numprocs, num_of_elements_per_process_received,chunk_beginner_element_received;
    /* start up MPI */

    MPI_Status status; 
    MPI_Init( &argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    
    /*synchronize all processes*/
    MPI_Barrier( MPI_COMM_WORLD );
    double tbeg = MPI_Wtime();
    /* write your code here */
    
    if(rank == 0) { //For the Master/Main process to do its computation and send and receive the computed parts from other slave processes
        
        //freopen(argv[1], "r", stdin);
        //freopen(argv[2], "a", stdout);
       
        FILE * file = NULL;
        int num_of_terms_in_fraction;
        //cin >> num_of_terms_in_fraction;

        file = fopen(argv[1], "r");
        fscanf(file, "%d", &num_of_terms_in_fraction);
        
        double sum = 0.0;
        int num_of_elements_per_process, chunk_beginner_element, last_chunk_size;

        if(num_of_terms_in_fraction >= numprocs) {
            if(num_of_terms_in_fraction%numprocs == 0) {
                num_of_elements_per_process = num_of_terms_in_fraction/numprocs;
                last_chunk_size = num_of_elements_per_process;
            }
            else {
                num_of_elements_per_process = num_of_terms_in_fraction/numprocs;
                last_chunk_size = num_of_elements_per_process + (num_of_terms_in_fraction%numprocs);
            }
        }
        else {
            num_of_elements_per_process = 1;
            last_chunk_size = 0;
        }
        
        if(numprocs > 1){
            int pid = 1;
            for(pid = 1; pid < numprocs-1 ; pid++) {
                
                chunk_beginner_element = (pid * num_of_elements_per_process) + 1;

                    if(pid < num_of_terms_in_fraction){
                        MPI_Send(&num_of_elements_per_process, 1, MPI_INT, pid, 0, MPI_COMM_WORLD);
                    }
                    else{
                        int temp = 0;
                        // num_of_elements_per_process = 0;
                        MPI_Send(&temp, 1, MPI_INT, pid, 0, MPI_COMM_WORLD);
                    }                   
                    MPI_Send(&chunk_beginner_element, 1, MPI_INT, pid, 0, MPI_COMM_WORLD);
            }

            chunk_beginner_element = (pid * num_of_elements_per_process) + 1;
            MPI_Send(&last_chunk_size, 1, MPI_INT, pid, 0, MPI_COMM_WORLD );
            MPI_Send(&chunk_beginner_element, 1, MPI_INT, pid, 0, MPI_COMM_WORLD);
        }
        
        //Master Process's computation
        for(int num = 1; num <= num_of_elements_per_process; num++) {
            sum += (1/pow(num,2));
        }
        
        //Receiving the computed protions from the child/slave processes
        if(numprocs > 1) {
            double temporary_sum_received = 0.0;
            for(int pid = 1; pid <= numprocs-1; pid++) {
                MPI_Recv(&temporary_sum_received, 1, MPI_DOUBLE, pid, 0, MPI_COMM_WORLD, &status);
                sum += temporary_sum_received;
            }
        }
        file = fopen(argv[2], "w");
        fprintf(file, "%0.6f", sum);
        fclose(file);
        //printResult(sum);
    }
    //For the Child processes to their computations
    else { 
        //Receiving the number of elements per process
        MPI_Recv(&num_of_elements_per_process_received, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        //Receiving the chunk beginner element per process
        MPI_Recv(&chunk_beginner_element_received, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

        double temporary_sum = 0.0;
        for(int i = 1; i <= num_of_elements_per_process_received; i++) {
            temporary_sum += (1/pow(chunk_beginner_element_received,2));
            chunk_beginner_element_received++;
        }

        MPI_Send(&temporary_sum, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    }
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