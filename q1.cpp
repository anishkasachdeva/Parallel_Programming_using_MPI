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
    printf("Sum for the series i.e. estimated value of π2/6 rounded oﬀ to six decimal places is : %.6f\n", sum);
}

int main( int argc, char **argv ) {
    
    int rank, numprocs, num_of_elements_per_process_received,chunk_beginner_element_received;
    /* start up MPI */

    MPI_Status status; 
    MPI_Init( &argc, &argv);

    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &numprocs );
    
    /*synchronize all processes*/
    MPI_Barrier( MPI_COMM_WORLD );
    double tbeg = MPI_Wtime();
    /* write your code here */

    /* Basic Implementation 

    Main/master process will be divided into multiple child processes.
    Now the master process "sends" the data to the other child/slave processes for their computations.
    Each process will do their part of computation in parallel.
    And in the end, master process after completing its own part/computation "receives" the result of respective process's computation
    and hence done. So this is the msg passing happening in parallel algorithms.
    
    */
    int num_of_terms_in_fraction = 100;
    if(rank == 0) { //For the Master/Main process to do its computation and send and receive the computed parts from other slave processes
        double sum = 0.0;
        int num_of_elements_per_process, chunk_beginner_element;

        if(num_of_terms_in_fraction >= numprocs) {
            if(num_of_terms_in_fraction%numprocs == 0)
                num_of_elements_per_process = num_of_terms_in_fraction/numprocs;
            else
            {
                num_of_elements_per_process = (num_of_terms_in_fraction/numprocs) + 1;
            }
        }
        else { //Handle this case 
    
        }
        
        if(numprocs > 1)
        {
            int pid = 1;
            for(pid = 1; pid < numprocs-1 ; pid++) {
                chunk_beginner_element = (pid * num_of_elements_per_process) + 1;
                    MPI_Send(
                        &num_of_elements_per_process, //Start
                        1, //Count
                        MPI_INT, //Datatype
                        pid, //Destination
                        0, //Tag
                        MPI_COMM_WORLD //Communicator
                    );
                    MPI_Send(
                        &chunk_beginner_element,
                        1,
                        MPI_INT,
                        pid,
                        0,
                        MPI_COMM_WORLD
                    );
            }

            int remaining_elements;
            chunk_beginner_element = (pid * num_of_elements_per_process) + 1;
            remaining_elements = (num_of_terms_in_fraction - chunk_beginner_element) + 1;

            MPI_Send(
                        &remaining_elements, //Start
                        1, //Count
                        MPI_INT, //Datatype
                        pid, //Destination
                        0, //Tag
                        MPI_COMM_WORLD //Communicator
                    );
                    MPI_Send(
                        &chunk_beginner_element,
                        1,
                        MPI_INT,
                        pid,
                        0,
                        MPI_COMM_WORLD
                    );
        }

        
        //Master Process's computation
        for(int num = 1; num <= num_of_elements_per_process; num++) {
            sum += (1/pow(num,2));
        }
        
        //Receiving the computed protions from the child/slave processes
        if(numprocs > 1) {
            double temporary_sum_received = 0.0;
            for(int pid = 1; pid <= numprocs-1; pid++)
            {
                MPI_Recv(
                    &temporary_sum_received,
                    1,
                    MPI_DOUBLE,
                    pid,
                    0,
                    MPI_COMM_WORLD,
                    &status
                );
                sum += temporary_sum_received;
            }
        }
        printResult(sum);
    }
    else { //For the Child processes to their computations
        MPI_Recv( //Receiving the number of elements per process
            &num_of_elements_per_process_received, //Start
            1, //Count
            MPI_INT, //Datatype
            0, //Source
            0, //Tag
            MPI_COMM_WORLD, //Communicator
            &status //Status
        );
        MPI_Recv( //Receiving the chunk beginner element per process
            &chunk_beginner_element_received,
            1,
            MPI_INT,
            0,
            0,
            MPI_COMM_WORLD,
            &status
        );

        double temporary_sum = 0.0;
        for(int i = 1; i <= num_of_elements_per_process_received; i++) {
            // if(chunk_beginner_element_received <= num_of_terms_in_fraction)
            // {
                temporary_sum += (1/pow(chunk_beginner_element_received,2));
            // }
            // else
            // {
                // break;
            // }
            chunk_beginner_element_received++;
        }

        MPI_Send(
            &temporary_sum,
            1,
            MPI_DOUBLE,
            0,
            0,
            MPI_COMM_WORLD
        );
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