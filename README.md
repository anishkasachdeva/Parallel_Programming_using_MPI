## Distributed Systems - Assignment 1 
####  Parallel Programming in MPI
---
##### Anishka Sachdeva (2018101112)
###### 22nd January, 2021
---
## Description and Analysis of Solutions
---
#### Question 1 :
###### Use the numerical identity that the sum of the reciprocals of the squares of integers converges to π2/6.
##### Approach :


---
#### Question 2
###### Given an array of numbers, your task is to return the array in sorted order by implementing parallel quicksort.
##### Approach :

Merging :
1. Create an output array.
2. Create a min heap of size k and insert 1st element in all the arrays into the heap.
3. Repeat following steps while priority queue is not empty.
    a) Remove minimum element from heap (minimum is always at root) and store it in output array.
    b) Insert next element from the array from which the element is extracted. If the array doesn’t have any more elements, then do nothing.

    The time complexity of heap based merging is O(N Log k) where k is the size of the heap. 


---
#### Question 3
###### Given an undirected graph G, ﬁnd a proper edge coloring of the graph using Delta(G) + 1 colors or fewer. No 2 adjacent edges should have a same color. Delta(G) is the maximum degree of any vertex in G.
##### Approach : 


---