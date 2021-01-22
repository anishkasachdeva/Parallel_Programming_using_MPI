## Distributed Systems - Assignment 1 
####  Parallel Programming in MPI
---
##### Anishka Sachdeva (2018101112)
###### 22nd January, 2021
---
## Description and Analysis of Solutions
---
#### Question 1 :
##### Use the numerical identity that the sum of the reciprocals of the squares of integers converges to π2/6.
#### Implementation Approach

##### Basic Strategy :
Main/master process will be divided into multiple child processes. Now the master process "sends" the data to the other child/slave processes for their computations. Each process will do their part of computation in parallel. And in the end, master process after completing its own part/computation "receives" the result of respective process's computation and hence done. So this is the msg passing happening in parallel algorithms.
---
#### Question 2 :
##### Given an array of numbers, your task is to return the array in sorted order by implementing parallel quicksort.
#### Implementation Approach

###### Division :
1. The input array is divided into multiple equal chunks (size of array/number of processes).
2. Each small array chunk is assigned to a process.
3. Each process parallelly sorts the chunk of array allocated to it.
4. The MPI Commands used are : 
    1. MPI_Send
    2. MPI_Recv
###### Sorting :
1. Algorithm used for sorting is Quick Sort Algorithm.
###### Merging :
1. Create an output array.
2. Create a min heap of size k (using Priority Queue of C++ STL Library) and insert 1st element in all the arrays into the heap.
3. Repeat following steps while priority queue is not empty.
    1. Remove minimum element from heap (minimum is always at root) and store it in output array.
    2. Insert next element from the array from which the element is extracted. If the array doesn’t have any more elements, then do nothing.

    The time complexity of heap based merging is O(N Log k) where k is the size of the min heap. 


---
#### Question 3 :
##### Given an undirected graph G, ﬁnd a proper edge coloring of the graph using Delta(G) + 1 colors or fewer. No 2 adjacent edges should have a same color. Delta(G) is the maximum degree of any vertex in G.
#### Implementation Approach 

---