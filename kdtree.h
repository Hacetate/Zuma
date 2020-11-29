#pragma once

#include "ball.h"

//2d
#define k 2

// A structure to represent node of kd tree 
struct kdnode
{
    DataType data;
    int split;
    kdnode* left, * right;
};
typedef struct Node
{
    DataType data;
    struct Node* next;
} Node;

struct kdnode* newNode(DataType data,unsigned depth);
kdnode* kdnode_insert(kdnode* root, DataType data);
kdnode* findMin(kdnode* root, int d);
kdnode* kdnode_delete(kdnode* root, DataType data);
void searchNearest(kdnode* root, DataType target, DataType& nearestdata, float& distance); 
void kdtree_destroy(kdnode* root);
Node* CreateEmptyList();
void ListInsert(Node* head, unsigned int index, DataType data);
DataType ListDelete(Node* head, unsigned int index);


