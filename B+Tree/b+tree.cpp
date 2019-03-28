#include <bits/stdc++.h>
#include "node.h"
using namespace std;

class Node {
public:
    int numKeys;
    int t;
    int *keys;
    
    Node(int t) {
        this->t = t;
        this->numKeys = 0;
    }
};

class IndexNode : public Node {
public:
    void **ptrs;
    IndexNode(int t) : Node(t) {
        keys = new int[2*t-1];
        ptrs = new void*[2*t];
    }
};

class DataNode : public Node {
public:
    DataNode *left, *right;
    DataNode(int t) : Node(t) {
        keys = new int[2*t+1];
        left = NULL;
        right = NULL; 
    }
    void InsertKey(int key) {
        for (int i = this->numKeys-1; i >= 0; i--) {
            if (this->keys[i] > key) {
                this->keys[i+1] = this->keys[i]; // shift to make room for key
            }
            if (this->keys[i] <= key)  {
                this->keys[i+1] = key; // place key
                break;
            }
            if (i == 0) {
                this->keys[i] = key;
                break;
            }
        }
        this->numKeys++;
    }
};

class BPlusTree {
public:
    int depth;
    int t;
    void * root;
    BPlusTree(int t) {
        depth = 0;
        this->t = t;
        this->root = new DataNode(t);
    }
    DataNode *search(void *node, int key, int depth) {
        if (depth == 0) { // reached appropriate leaf datanode
            DataNode *curNode = (DataNode *) node;
            for (int i = 0; i < curNode->numKeys; i++) {
                if (key == curNode->keys[i]) {
                    return curNode;
                }
            }
            return NULL;  
        }   
        else { // direct towards appropriate leaf datanode
            IndexNode *curNode = (IndexNode *) node;
            for (int i = 0; i < curNode->numKeys; i++) {
                if (key <= curNode->keys[i]) {
                    return search(curNode->ptrs[i], key, depth--);
                }
                if (i == curNode->numKeys-1) {
                    return search(curNode->ptrs[i+1], key, depth--);
                }
            }
        }
    }
    void *insert(void *node, int key, int depth, int *up) {
        if (depth == 0) { // reached datanode, attempt insert
            DataNode *root = (DataNode *) node; 
            if (root->numKeys < 2 * this->t) { // insert to nonfull datanode 
                root->InsertKey(key);
                return (void *) root; // if no split return the same ptr;
            }
            else { // split leaf (data) node
                DataNode *newLeaf = NULL;
                this->splitDataNode((DataNode *) this->root, key, up, &newLeaf);
                this->depth++;
                // split successful now set base for backpropagation
                return (void *) newLeaf;
            }
        }
        else { // augment path search with back propagation
            IndexNode *curNode = (IndexNode *) node;
            int pos = curNode->numKeys;
            for (int i = 0; i < curNode->numKeys; i++) {
                if (key <= curNode->keys[i]) {
                    pos = i;
                    break;
                }
            }
            void *nextNode = curNode->ptrs[pos];
            void *newChild = insert(nextNode, key, depth-1, up);
            if (newChild == nextNode) {
                return (void *) curNode;
            }
            else { // need to split curNode which is an indexnode
                 
            }
        }
    }

    void splitDataNode(DataNode *node, int key, int *up, DataNode **pnewLeaf) {
        int t = node->t;
        node->InsertKey(key);
        assert(node->numKeys == 2*t+1);
        *pnewLeaf = new DataNode(t);
        *up = (node->keys[t] + node->keys[t+1])/2; 
        for (int i = 2*t; i > t; i--) {
            (*pnewLeaf)->keys[i-t-1] = node->keys[i];
        }
        node->numKeys = t+1;
        (*pnewLeaf)->numKeys = t;
        return;
    }

}

int main() {

}