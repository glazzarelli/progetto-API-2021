//
// Created by Gabriele Lazzarelli on 6/30/21.
//

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define COUNT 10
#define RED 'r'
#define BlACK 'b'
#define MAXLINESIZE 20384
#define ADD 'A'
#define TOP 'T'

//global variables
int offset;
int matrixSize;
int key = 0;

//Heap values
typedef struct elem{
    int weight;
    int index;
    int prev;
}elem;

elem heap[MAXLINESIZE/4];
int heapIndex[MAXLINESIZE/4];
int heapSize = 0;

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
    if (n > heapSize/2){
        return;
    }
    int l = left(n);
    int r = right(n);
    int posMin = n;
    if (l < heapSize && heap[l].weight < heap[n].weight){
        posMin = l;
    }
    if (r < heapSize && heap[r].weight < heap[posMin].weight){
        posMin = r;
    }
    if (posMin == n){
        return;
    }
    elem tmp = heap[n];

    heapIndex[heap[posMin].index] = n;
    heapIndex[tmp.index] = posMin;

    heap[n] = heap[posMin];
    heap[posMin] = tmp;

    minHeapify(posMin);
}

elem deleteMin(){
    if (heapSize < 1){
        elem zero;
        zero.weight = 0;
        return zero;
    }
    elem min = heap[0];
    heapSize--;

    heapIndex[min.index] = heapSize;
    heapIndex[heap[heapSize].index] = 0;

    heap[0] = heap[heapSize];
    heap[heapSize] = min;
    minHeapify(0);
    return min;
}

void insertHeap(int weight, int index){
    heap[heapSize].weight = weight;
    heap[heapSize].index = index;
    heap[heapSize].prev = -1;
    heapIndex[index] = heapSize;
    int i = heapSize;
    heapSize++;
    elem tmp;
    while (i > 0 && heap[parent(i)].weight > heap[i].weight){
        tmp = heap[i];

        heapIndex[heap[parent(i)].index] = i;
        heapIndex[tmp.index] = parent(i);

        heap[i] = heap[parent(i)];
        heap[parent(i)] = tmp;
        i = parent(i);
    }
}

void buildMinHeap(){
    for (int i = heapSize/2; i >= 0; --i) {
        minHeapify(i);
    }
}

/*
 * node RB tree
 */
typedef struct node{
    struct node *parent;
    struct node *left;
    struct node *right;
    char color;
    unsigned int sum;
    int idx;
}*node;
node root = NULL;

node createNode(unsigned int sum){
    node t = malloc(sizeof(struct node));
    t->parent = NULL;
    t->left = NULL;
    t->right = NULL;
    t->color = RED;
    t->sum = sum;
    t->idx = key;
    key++;
    return t;
}

/*
 * RB tree implementation
 */
void inOrder(node t){
    if (t == NULL) {
        return;
    }
    inOrder(t->left);
    printf("%d%c ", t->sum, t->color);
    inOrder(t->right);
}

void leftRotate(node x){
    node y = x->right;
    x->right = y->left;
    if (y->left != NULL){
        y->left->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == NULL){
        root = y;
    } else if (x == x->parent->left){
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}

void rightRotate(node y){
    node x = y->left;
    y->left = x->right;
    if (x->right != NULL){
        x->right->parent = y;
    }
    x->parent = y->parent;
    if (y->parent == NULL){
        root = x;
    } else if (y == y->parent->right){
        y->parent->right = x;
    } else {
        y->parent->left = x;
    }
    x->right = y;
    y->parent = x;
}

void insertFixUp(node t){
    while (t->parent->color == RED){
        if (t->parent == t->parent->parent->left){
            node r = t->parent->parent->right;
            if (r != NULL && r->color == RED){
                t->parent->color = BlACK;
                r->color = BlACK;
                t->parent->parent->color = RED;
                t = t->parent->parent;
                if (t->parent == NULL || t->parent->parent == NULL){
                    return;
                }
            } else if (t == t->parent->right){
                t = t->parent;
                leftRotate(t);
            } else {
                t->parent->color = BlACK;
                t->parent->parent->color = RED;
                rightRotate(t->parent->parent);
            }
        } else {
            node r = t->parent->parent->left;
            if (r != NULL && r->color == RED){
                t->parent->color = BlACK;
                r->color = BlACK;
                t->parent->parent->color = RED;
                t = t->parent->parent;
                if (t->parent == NULL || t->parent->parent == NULL){
                    return;
                }
            } else if (t == t->parent->left){
                t = t->parent;
                rightRotate(t);
            } else {
                t->parent->color = BlACK;
                t->parent->parent->color = RED;
                leftRotate(t->parent->parent);
            }
        }
    }
}

void insert(node t){
    node pre = NULL;
    node cur = root;
    while (cur != NULL){
        pre = cur;
        if (t->sum < cur->sum){
            cur = cur->left;
        } else {
            cur = cur->right;
        }
    }
    t->parent = pre;
    if (pre == NULL){
        root = t;
    } else if (t->sum < pre->sum){
        pre->left = t;
    } else {
        pre->right = t;
    }
    if (t->parent != NULL && t->parent->parent != NULL){
        insertFixUp(t);
    }
    root->color = BlACK;
}

void print2DUtil(node pNode, int space)
{
    // Base case
    if (pNode == NULL)
        return;

    // Increase distance between levels
    space += COUNT;

    // Process right child first
    print2DUtil(pNode->right, space);

    // Print current node after space
    // count
    printf("\n");
    for (int i = COUNT; i < space; i++)
        printf(" ");
    printf("%d%c\n", pNode->sum, pNode->color);

    // Process left child
    print2DUtil(pNode->left, space);
}
void print2D(node pNode)
{
    // Pass initial space count as 0
    print2DUtil((node) pNode, 0);
}

char getCommand(){
    char command;
    char c = getchar_unlocked();
    while (c != ADD && c != TOP) c = getchar_unlocked();
    command = c;
    while(c != '\n') c = getchar_unlocked();
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

int getIntFrom(const char *string){
    int ret = 0;

    while(string[offset] < 48 || string[offset] > 57){
        offset++;
    }
    while(string[offset] > 47 && string[offset] < 58){
        ret = 10*ret+string[offset]-48;
        offset++;
    }

    return ret;
}

int *readMatrix(){
    char *string = malloc(MAXLINESIZE * sizeof(char));
    fgets(string, MAXLINESIZE, stdin);

    matrixSize = 1;
    offset = 0;
    while (string[offset] != '\n'){
        if (string[offset] == ','){
            matrixSize++;
        }
        offset++;
    }

    offset = 0;
    int *matrix = malloc(matrixSize * matrixSize * sizeof(int));

    for (int i = 0; i < matrixSize; ++i) {
        matrix[i] = getIntFrom(string);
    }
    for (int i = matrixSize; i < matrixSize * matrixSize; ++i) {
        matrix[i] = getInt();
    }

    return matrix;
}

int sumShortestPaths(const int *mat){
    heapSize = 0;
    int rowIndex;
    int sum = 0;
    elem min;

    /*Initialize values in the heap:
     * - source is 0, distance from 0 is zero
     * - the rest of the nodes has dist = INT_MAX
     * Note that each node has an attribute rowIndex.
     * heapIndex creates a correspondence between each node and it position in the heap:
     */
    if (matrixSize < 2){
        return 0;
    } else {
        insertHeap(0, 0);
    }
    for (int i = 1; i < matrixSize; ++i) {
        insertHeap(INT_MAX,i);
    }

    /* Applying Dijkstra's algorithm:
     * - takes the x element non visited with minor dist
     * - calculates its row in the matrix
     * - for each node reachable from x:
     *      calculate the newWeight = dist from source + dist(x,node)
     *      if newWeight < i-node.sum -> update value
     * - buildMinHeap to restore the priority queue3
     */
    while (heapSize != 0){
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
            if (newWeight < heap[heapIndex[i]].weight){
                heap[heapIndex[i]].weight = newWeight;
            }
        }
        buildMinHeap();
    }

    //! heap[heapIndex[i]] -> heap[i]
    for (int i = 0; i < matrixSize; ++i) {
        if (heap[heapIndex[i]].weight != INT_MAX){
            sum += heap[heapIndex[i]].weight;
        }
    }

    return sum;
}

void addGraph(){
    int *mat = readMatrix();
    int sum = sumShortestPaths(mat);
}
int main(){
    int d,k;
    char input = 'z';
    int *mat;
    int tmp;
    while (input != 'q'){
        input = getchar_unlocked();
        switch (input) {
            case 'a': //insert into RB tree
                insert(createNode(getInt()));
                break;
            case 'b': //print in order RB tree
                inOrder(root);
                break;
            case 'c': //print 2D tree
                print2D(root);
                break;
            case 'd':
                mat = readMatrix();
                for (int i = 0; i < matrixSize * matrixSize; ++i) {
                    printf("%d ", mat[i]);
                }
                printf("\n");
                break;
            case 'e':
                tmp = heapSize;
                for (int i = 0; i < tmp; ++i) {
                    printf("%d ", deleteMin().weight);
                }
                break;
            case 'f':
                mat = readMatrix();
                tmp = sumShortestPaths(mat);
                printf("%d\n", tmp);
                break;
            default: break;
        }
    }
    return 0;
}



