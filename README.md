# Final Project for Algorithms and Data Structures

## Educational Objectives and Implementation

### Objectives
- Practical application of techniques learned in the algorithms and data structures module of the Algorithms and Computer Science Principles course
- Implementation of a solution to a problem, paying attention to concrete aspects of code efficiency

### Implementation
- C language (C11, VLAs allowed)
- No external libraries beyond the standard C library
- No multithreading
- Input data received via stdin, results to be provided via stdout


## GraphRanker

The objective of this project is to manage a ranking among weighted directed graphs.

- The ranking keeps track of the k "best" graphs
- The program to be implemented receives as input:
  - Two parameters, only once (on the first line of the file, separated by space)
    - d: the number of nodes in the graphs
    - k: the length of the ranking
  - A sequence of commands among:
    - AddGraph [adjacency-matrix]
    - TopK

d, k, and the number of graphs are representable with 32-bit integers.

### AddGraph
Requires adding a graph to those considered for ranking. It is followed by the adjacency matrix of the graph itself, printed one row per line, with elements separated by commas.

The nodes of the graph are to be considered logically labeled with an integer index between 0 and d-1; the node in position 0 is the one whose outgoing star is described by the first row of the matrix.

The weights of the graph edges are integers in the range [0, 2^32 - 1].

Example for d=3:
```
AddGraph
3,7,42
0,7,2
7,4,3
```

### TopK
- Consider each graph from the beginning of the program up to the TopK command labeled with an integer index corresponding to the number of graphs read before it (starting from 0)
- TopK requires the program to print the integer indices of the k graphs having the k smallest values of the following metric:
  - Sum of the shortest paths between node 0 and all other nodes of the graph reachable from 0
- If there are multiple graphs with the same metric value, priority is given to those that arrived first
- The distances of nodes not reachable from 0 are considered null
- The k integer indices are printed on a single line, separated by a space, in any order

## A Sample Execution

Input received:
```
3,2
AddGraph
0,4,3
0,2,0
2,0,0
AddGraph
0,0,2
7,0,4
0,1,0
AddGraph
3,1,8
0,0,5
0,9,0
TopK
```

Comments and Expected Output:
- Request to manipulate graphs with 3 nodes and report the k=2 best
- Addition of the first graph (index 0, sum of paths = 7)
- Addition of the second graph (index 1, sum of paths = 5)
- Addition of the third graph (index 2, sum of paths = 7)

Expected output: `0 1` or `1 0`

--- 

# GraphRanker Solution

## Key Components

### Data Structures
1. Min-Heap: Used for Dijkstra's algorithm
2. Max-Heap: Used for maintaining the top K graphs

### Main Functions

1. `sumShortestPaths`:
   - Implements Dijkstra's algorithm to calculate the sum of shortest paths from node 0 to all reachable nodes
   - Uses a min-heap for efficient selection of the next node to process

2. `addGraph`:
   - Reads the adjacency matrix of a new graph
   - Calculates the sum of shortest paths
   - Inserts the result into the max-heap

3. `topK`:
   - Prints the indices of the top K graphs based on the current max-heap state

### Optimization Techniques

1. Custom input parsing:
   - `getCommand()` and `getInt()` functions for efficient input reading
   - Avoids the overhead of standard I/O functions

2. Heap operations:
   - Custom implementations of min-heap and max-heap operations
   - Efficient insertion and deletion of elements

3. Memory management:
   - Dynamic allocation of the adjacency matrix
   - Proper freeing of allocated memory after use

## Algorithm Complexity

- Time complexity for adding a graph: O(V^2 log V), where V is the number of nodes
  - Dominated by Dijkstra's algorithm with binary heap
- Space complexity: O(V^2) for storing the adjacency matrix


This solution provides an efficient implementation of the GraphRanker problem, balancing time and space complexity while adhering to the project requirements.


