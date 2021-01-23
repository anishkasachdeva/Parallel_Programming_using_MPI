## Distributed Systems - Assignment 1 
####  Parallel Programming in MPI
---
##### Anishka Sachdeva (2018101112)
###### 23rd January, 2021
---
## Description and Analysis of Solutions
---
#### Question 1 :
##### Use the numerical identity that the sum of the reciprocals of the squares of integers converges to π2/6.
#### Implementation Approach

###### Basic Strategy :
1. Main/master process will be divided into multiple child processes known as slave processes.
2. Now the master process "sends" the data to the other child/slave processes for their computations.
3. Each process will do their part of computation in parallel.
4. In the end, master process after completing its own part/computation "receives" the result of respective process's computation and hence done. So this is the message passing happening in parallel algorithms.

###### Implementation Strategy :
1. The master process sends start number from where each process has start the computation parallelly.
2. On receiving the start number, the child/slave processes calculate their respective sum and sends back to the master process/
3. The master process also computes its respective sum until then.
4. The master process then receives the patial sums from all the other respective processes and computes the final sum. 

###### Major MPI Commands Used :
1. MPI_Send
2. MPI_Recv 
---
#### Question 2 :
##### Given an array of numbers, your task is to return the array in sorted order by implementing parallel quicksort.
#### Implementation Approach
###### Division :
1. The input array is divided into multiple equal chunks (size of array/number of processes).
2. Each small array chunk is assigned to a process.
3. Each process parallelly sorts the chunk of array allocated to it.
   
###### Sorting :
1. Quick Sort Algorithm is used for sorting the individual chunk of array.
###### Merging :
1. Create an output array which will store the final sorted array values.
2. Create a min heap of size k (using Priority Queue of C++ STL Library) and insert 1st element in all the arrays into the heap.
3. Repeat following steps while priority queue is not empty.
    1. Remove minimum element from heap (minimum is always at root) and store it in output array.
    2. Insert next element from the array from which the element is extracted. If the array doesn’t have any more elements, then do nothing.

    The time complexity of heap based merging is O(N Log k) where k is the size of the min heap. 


###### Major MPI Commands Used :
1. MPI_Send
2. MPI_Recv 

---
#### Question 3 :
##### Given an undirected graph G, ﬁnd a proper edge coloring of the graph using Delta(G) + 1 colors or fewer. No 2 adjacent edges should have a same color. Delta(G) is the maximum degree of any vertex in G.
#### Implementation Approach 

###### Basic Strategy :
1. The basic strategy is to convert the original graph into a line graph. Given a graph G, its line graph L(G) is a graph such that. Each vertex of L(G) represents an edge of G; and. two vertices of L(G) are adjacent if and only if their corresponding edges share a common endpoint ("are incident") in G.
2. Now the colouring of the line graph can be done using delta(L(G))+1 colors where L(G) represents the line graph of original graph and delta(L(G)) represents the maximum degree of the line graph.
3. Now the problem gets converted to coloring the vertices of the line graph using atmost delta(L(G)) + 1 colors.

###### Implementation Strategy :
1. Used the Jones - Plassmann Algorithm to implement the vertex coloring algorithm. The algorithm states : 
    1. Initially assign random numbers to all the vertices. Instead of assigning random numbers, i have assigned numbers from 0 to vertices -1.
    2. Each vertex looks at its neighbours and if it had the highest number, it gets assigned the lowest available color.
    3. Each uncoloured vertex looks at its uncoloured neighbours and gets colored with the lowest available color if has the highest number, and so on.
2. Initially the adjacency matrix of the Line Graph is broadcasted to all the processes. The adjancency matrix is built using a map which contains the information about the original graph.
3. Each process gets assigned equal number of vertices of the graph which they are supposed to color.
4. Then each process parallelly implements the Jones-Plassann Algorithm and color the vertices.
5. Then the changes done by each process in the final color array parallelly is sent to the process with rank 0 to update the final color array.
6. The master process receives individual final color array from all the processes and updates the final color array completeley i.e. it incorporates all the changes of the colored vertices.
7. Finally the final color array is broadcasted to all the processes so that there remains consistency globally i.e. each process gets the latest updated final color array.
8. This process goes on until all the vertices are colored.

###### Major MPI Commands Used : 
1. MPI_Bcast 
2. MPI_Send
3. MPI_Recv

---