//
// Created by Gabriele Lazzarelli on 6/30/21.
//

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define HEAPSIZE 20384
#define ADD 'A'
#define TOP 'T'

//global variables
int k;

typedef struct elem{
    int weight;
    int index;
}elem;

//Dijkstra minHeap
elem minHeap[HEAPSIZE];
int minHeapIndex[HEAPSIZE];
int minHeapSize = 0;


//Shortest paths maxHeap
elem maxHeap[HEAPSIZE];
int maxHeapSize = 0;

/*
 * Min-Heap
 */
int parent(int n){
    return (n-1)/2;
}

int left(int n){
    return 2*n+1;
}

int right(int n){
    return 2*n+2;
}

void minHeapify(int n){
    if (n > minHeapSize / 2){
        return;
    }
    int l = left(n);
    int r = right(n);
    int posMin = n;
    if (l < minHeapSize && minHeap[l].weight < minHeap[n].weight){
        posMin = l;
    }
    if (r < minHeapSize && minHeap[r].weight < minHeap[posMin].weight){
        posMin = r;
    }
    if (posMin == n){
        return;
    }
    elem tmp = minHeap[n];

    minHeapIndex[minHeap[posMin].index] = n;
    minHeapIndex[tmp.index] = posMin;

    minHeap[n] = minHeap[posMin];
    minHeap[posMin] = tmp;

    minHeapify(posMin);
}

void maxHeapify(int n){
    if (n > maxHeapSize / 2){
        return;
    }
    int l = left(n);
    int r = right(n);
    int posMax = n;
    if (l < maxHeapSize && maxHeap[l].weight > maxHeap[n].weight){
        posMax = l;
    }
    if (r < maxHeapSize && maxHeap[r].weight > maxHeap[posMax].weight){
        posMax = r;
    }
    if (posMax == n){
        return;
    }
    elem tmp = maxHeap[n];

    maxHeap[n] = maxHeap[posMax];
    maxHeap[posMax] = tmp;

    maxHeapify(posMax);
}

elem deleteMin(){
    if (minHeapSize < 1){
        elem zero;
        zero.weight = 0;
        return zero;
    }
    elem min = minHeap[0];
    minHeapSize--;

    minHeapIndex[min.index] = minHeapSize;
    minHeapIndex[minHeap[minHeapSize].index] = 0;

    minHeap[0] = minHeap[minHeapSize];
    minHeap[minHeapSize] = min;
    minHeapify(0);
    return min;
}

elem deleteMax(){
    elem max = maxHeap[0];
    maxHeapSize--;

    maxHeap[0] = maxHeap[maxHeapSize];
    maxHeapify(0);
    return max;
}

void insertMinHeap(int weight, int index){
    minHeap[minHeapSize].weight = weight;
    minHeap[minHeapSize].index = index;

    minHeapIndex[index] = minHeapSize;
    int i = minHeapSize;
    minHeapSize++;
    elem tmp;

    while (i > 0 && minHeap[parent(i)].weight > minHeap[i].weight){
        tmp = minHeap[i];

        minHeapIndex[minHeap[parent(i)].index] = i;
        minHeapIndex[tmp.index] = parent(i);

        minHeap[i] = minHeap[parent(i)];
        minHeap[parent(i)] = tmp;
        i = parent(i);
    }
}

void insertMaxHeap(int weight, int index){
    if (maxHeapSize == k + 1){
        deleteMax();
    }

    maxHeap[maxHeapSize].weight = weight;
    maxHeap[maxHeapSize].index = index;

    int i = maxHeapSize;
    maxHeapSize++;
    elem tmp;

    while (i > 0 && maxHeap[parent(i)].weight <= maxHeap[i].weight){
        tmp = maxHeap[i];

        maxHeap[i] = maxHeap[parent(i)];
        maxHeap[parent(i)] = tmp;
        i = parent(i);
    }
}

void buildMinHeap(){
    for (int i = minHeapSize / 2; i >= 0; --i) {
        minHeapify(i);
    }
}

char getCommand(){
    char command;
    char c = getchar_unlocked();
    while (c != EOF && c != ADD && c != TOP) c = getchar_unlocked();
    command = c;
    if (c != EOF){
        while(c != '\n') c = getchar_unlocked();
    }
    return command;
}

int getInt(){
    int ret = 0;
    int i = getchar_unlocked();
    while(i < 48 || i > 57){
        i = getchar_unlocked();
    }
    while(i > 47 && i < 58){
        ret = 10*ret+i-48;
        i = getchar_unlocked();
    }
    return ret;
}

int *readMatrix(int matrixSize){
    int *matrix = malloc(matrixSize * matrixSize * sizeof(int));

    for (int i = 0; i < matrixSize * matrixSize; ++i) {
        matrix[i] = getInt();
    }

    return matrix;
}

int sumShortestPaths(const int *mat, int matrixSize){
    minHeapSize = 0;
    int rowIndex;
    int sum = 0;
    elem min;

    /*Initialize values in the minHeap:
     * - source is 0, distance from 0 is zero
     * - the rest of the nodes has dist = INT_MAX
     * Note that each node has an attribute rowIndex.
     * minHeapIndex creates a correspondence between each node and it position in the minHeap:
     */

    insertMinHeap(0, 0);
    for (int i = 1; i < matrixSize; ++i) {
        insertMinHeap(INT_MAX, i);
    }

    /* Applying Dijkstra's algorithm:
     * - takes the x element non visited with minor dist
     * - calculates its row in the matrix
     * - for each node reachable from x:
     *      calculate the newWeight = dist from source + dist(x,node)
     *      if newWeight < i-node.sum -> update value
     * - buildMinHeap to restore the priority queue
     */
    while (minHeapSize != 0){
        min = deleteMin();
        if (min.weight == INT_MAX){
            break;
        }
        rowIndex = matrixSize * min.index;
        for (int i = 1; i < matrixSize; ++i) {
            if (mat[rowIndex + i] == 0){
                continue;
            }
            int newWeight = min.weight + mat[rowIndex + i];
            if (newWeight < minHeap[minHeapIndex[i]].weight){
                minHeap[minHeapIndex[i]].weight = newWeight;
            }
        }
        buildMinHeap();
    }

    for (int i = 0; i < matrixSize; ++i) {
        if (minHeap[i].weight != INT_MAX){
            sum += minHeap[i].weight;
        }
    }

    return sum;
}

void addGraph(int matrixSize, int *index){
    int *mat = readMatrix(matrixSize);
    int sum = sumShortestPaths(mat, matrixSize);
    insertMaxHeap(sum, *index);
    (*index)++;
    free(mat);
}

void topK(){
    if (k < maxHeapSize){
        for (int i = 1; i < maxHeapSize - 1; ++i) {
            printf("%d ", maxHeap[i].index);
        }
    } else {
        for (int i = 0; i < maxHeapSize - 1; ++i) {
            printf("%d ", maxHeap[i].index);
        }
    }
    if (maxHeapSize == 0){
        printf("\n");
    } else {
        printf("%d\n",maxHeap[maxHeapSize-1].index);
    }
}

int main(){
    int index = 0;
    int d;
    char c;
    d = getInt(); //number of nodes
    k = getInt(); //top graphs to display
    while (1) {
        c = getCommand();
        switch (c) {
            case 'A':
                addGraph(d, &index);
                break;
            case 'T':
                topK();
                break;
            case EOF:
                return 0;
        }
    }
}
