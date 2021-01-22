/* MPI Program Template */
#include <stdio.h>
#include <string.h>
#include "mpi.h"
#include <fstream>
#include <bits/stdc++.h>
using namespace std;
typedef long long int ll;


void swap(int *xp, int *yp)  
{  
    int temp = *xp;  
    *xp = *yp;  
    *yp = temp;  
}


int partition (int arr[], int low, int high)  
{  
    int pivot = arr[high];  
    int i = (low - 1);
  
    for (int j = low; j <= high - 1; j++)  
    {  
        if (arr[j] < pivot)  
        {  
            i++;
            swap(&arr[i], &arr[j]);  
        }  
    }  
    swap(&arr[i + 1], &arr[high]);  
    return (i + 1);  
}  

void quickSort(int arr[], int low, int high)  
{  
    if (low < high)  
    { 
        int pi = partition(arr, low, high);  
        quickSort(arr, low, pi - 1);  
        quickSort(arr, pi + 1, high);  
    }  
}  


vector<int> mergeSortedArrays(vector<vector<int> >&matrix_of_sorted_arrays)
{
    priority_queue<pair<int, pair<int, int> >, vector<pair<int, pair<int, int> >>, greater<pair<int, pair<int, int> >> > minheap; 
    
    for(int i = 0; i < matrix_of_sorted_arrays.size(); i++)
    {
        minheap.push({matrix_of_sorted_arrays[i][0],{i,0}});
    }
    vector<int>sorted_array;
    while(minheap.size() != 0)
    {
        pair<int, pair<int, int> > current = minheap.top();

        int current_row = current.second.first;
        int current_column = current.second.second;
        minheap.pop();
        if((current_column + 1 ) < matrix_of_sorted_arrays[current_row].size())
        {
            minheap.push({matrix_of_sorted_arrays[current_row][current_column+1],{current_row,current_column + 1}});
        }
        sorted_array.push_back(matrix_of_sorted_arrays[current_row][current_column]);
    }
    return sorted_array;
}


int main(int argc, char **argv )
{
    int rank, numprocs;

    MPI_Status status; 

    /* start up MPI */
    MPI_Init( &argc, &argv );

    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &numprocs );

    /*synchronize all processes*/
    MPI_Barrier( MPI_COMM_WORLD );
    double tbeg = MPI_Wtime();

    /* write your code here */

    if(rank == 0) {
        
        freopen(argv[1], "r", stdin);
        freopen(argv[2], "a", stdout);

        int size_of_input_array;
        cin >> size_of_input_array;

        int unsorted_array[size_of_input_array];

        for(int i = 0; i < size_of_input_array; i++)
        {
            cin >> unsorted_array[i];
        }

        int array_size_per_process, last_array_chunk_size;

        if(numprocs <= size_of_input_array)
        {
            if(size_of_input_array%numprocs == 0)
            {
                array_size_per_process = size_of_input_array/numprocs;
                last_array_chunk_size = array_size_per_process;
            }
            else
            {
                array_size_per_process = size_of_input_array/numprocs;
                last_array_chunk_size = array_size_per_process + (size_of_input_array%numprocs);
            }
        }
        else
        {
            array_size_per_process = 1;
            last_array_chunk_size = 1;
        }
        int sorted_array[size_of_input_array];

        if(numprocs > 1)
        {
            int pid = 1;
            for(pid = 1; pid < numprocs - 1; pid++)
            {
                if(pid < size_of_input_array - 1)
                {
                    MPI_Send(&array_size_per_process, 1, MPI_INT, pid, 0, MPI_COMM_WORLD);
                    MPI_Send(unsorted_array + (array_size_per_process * pid), array_size_per_process,MPI_INT, pid, 0, MPI_COMM_WORLD);
                }
                else
                {
                    int temp = 0;
                    MPI_Send(&temp, 1, MPI_INT, pid, 0, MPI_COMM_WORLD);
                }
            }

            MPI_Send(&last_array_chunk_size,1, MPI_INT, pid, 0, MPI_COMM_WORLD);
            MPI_Send(unsorted_array + (array_size_per_process * pid), last_array_chunk_size, MPI_INT, pid, 0, MPI_COMM_WORLD);
        }
        
        quickSort(unsorted_array, 0, array_size_per_process-1); //master process's sorting
        vector<vector<int> > matrix_of_sorted_arrays;
        
        if(numprocs > 1)
        {
            int pid = 1;
            for(pid = 1; pid < numprocs-1; pid++)
            {
                if(pid < size_of_input_array - 1)
                {
                    //Receive from np-2 child processes
                    MPI_Recv(unsorted_array + (array_size_per_process * pid), array_size_per_process, MPI_INT, pid, 0, MPI_COMM_WORLD, &status);
                }
                
            }
            //receive last sorted array chunk
            MPI_Recv( 
                unsorted_array + (array_size_per_process * pid), last_array_chunk_size, MPI_INT, numprocs-1, 0, MPI_COMM_WORLD, &status);
        }

        // for(int i = 0; i < size_of_input_array; i++)
        // {
        //     cout << unsorted_array[i] << " ";
        // }
        // cout << endl;

        int itr = 0;
        vector<int>v;
        for(int i = 0; i < numprocs - 1; i++)
        {
            if(i < size_of_input_array - 1)
            {
                for(int j = 0; j < array_size_per_process; j++)
                {
                    v.push_back(unsorted_array[itr++]);    
                }
                matrix_of_sorted_arrays.push_back(v);
                v.clear();
            }
        }

        while(itr < size_of_input_array)
        {
            v.push_back(unsorted_array[itr++]);
        }
        if(v.size() != 0)
            matrix_of_sorted_arrays.push_back(v);

        vector<int>arr_n = mergeSortedArrays(matrix_of_sorted_arrays);

        for(int i = 0; i < arr_n.size(); i++)
        {
            cout << arr_n[i] << endl;
        }
    }
    else
    {
        int size_of_received_array;
        MPI_Recv(&size_of_received_array, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        if(size_of_received_array != 0)
        {
            int received_array[size_of_received_array];
            MPI_Recv(received_array, size_of_received_array, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
            quickSort(received_array,0,size_of_received_array-1);
            MPI_Send(received_array, size_of_received_array, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
        
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