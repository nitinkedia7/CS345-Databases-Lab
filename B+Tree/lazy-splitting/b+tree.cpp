#include <bits/stdc++.h>
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
        this->keys = new int[2*t];
        this->ptrs = new void*[2*t+1];
        for (int i = 0; i < 2*t+1; i++) {
            this->ptrs[i] = NULL;
        }
    }
    void InsertNonFull(int key, int pos, void *newChild) {
        // assert(this->numKeys < 2*t-1);
        this->ptrs[this->numKeys+1] = this->ptrs[this->numKeys]; // shift rightmost ptr
        for (int i = this->numKeys-1; i >= pos; i--) { // shift keys and ptr till pos
            this->keys[i+1] = this->keys[i];
            this->ptrs[i+1] = this->ptrs[i];  
        }
        this->keys[pos] = key;
        this->ptrs[pos+1] = newChild;
        this->numKeys++;
        return;
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
        if (this->numKeys == 0) {
            this->keys[0] = key;
        }
        else {
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
        }
        this->numKeys++;
        return;
    }
};

class BPlusTree {
public:
    int depth;
    int t;
    void * root;
    BPlusTree(int t) {
        this->depth = 0;
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
    void insertmain(int key) {
        int up = INT_MAX;
        void *newChild = insert(this->root, key, this->depth, &up);
        if (this->root != newChild) {
            // make a new root;
            IndexNode *newRoot = new IndexNode(this->t);
            newRoot->numKeys++;
            newRoot->keys[0] = up;
            newRoot->ptrs[0] = this->root;
            newRoot->ptrs[1] = newChild;
            this->root = newRoot;
            this->depth++;
        }
        return;
    };
    void *insert(void *node, int key, int depth, int *up) {
        if (depth == 0) { // reached datanode, attempt insert
            DataNode *root = (DataNode *) node; 
            if (root->numKeys < 2 * this->t) { // insert to nonfull datanode 
                root->InsertKey(key);
                return (void *) root; // if no split return the same ptr;
            }
            else { // split leaf (data) node
                DataNode *newLeaf = NULL;
                this->splitDataNode(root, key, up, &newLeaf);
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
                if (curNode->numKeys < 2*t-1) {
                    curNode->InsertNonFull(*up, pos, newChild);
                    return (void *) curNode; 
                }
                else { // split this index node
                    IndexNode *newNonLeaf = NULL;
                    this->splitIndexNode(curNode, *up, pos, newChild, up, &newNonLeaf);
                    // split successful now set base for backpropagation
                    return (void *) newNonLeaf;
                } 
            }
        }
    }
    void splitIndexNode(IndexNode *node, int key, int pos, void *newChild, int *up, IndexNode **pnewNonLeaf) {
        int t = node->t;
        node->InsertNonFull(key, pos, newChild); // forcefully insert extra key since to make select middle easy
        assert(node->numKeys == 2*t);

        *pnewNonLeaf = new IndexNode(t);
        *up = node->keys[t];

        (*pnewNonLeaf)->ptrs[t-1] = node->ptrs[2*t]; 
        for (int i = 2*t-1; i > t; i--) {
            (*pnewNonLeaf)->keys[i-t-1] = node->keys[i];
            (*pnewNonLeaf)->ptrs[i-t-1] = node->ptrs[i];
        }
        node->numKeys = t;
        (*pnewNonLeaf)->numKeys = t-1;
        return;
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

    void LevelOrderTraversal(void *root, int depth) {
        queue<pair<int,void *> > q;
        queue<int> level;
        int nodeID = 1;
        pair<int, void*> curNode;
        int curLevel=0,prevLevel=-1;
        depth = depth + 1;
        q.push({nodeID, root});
        level.push(0);
        cout<<"\n*** NodeID :: Level :: Values ***\n"<<endl;
        while(!q.empty()){
            curNode = q.front(); q.pop();
            if(prevLevel!=level.front()){
                depth = depth-1;
                prevLevel = curLevel++;
            }
            level.pop();
            cout<<curNode.first<< " :: " << curLevel << " :: ";
            if(depth==0){
                DataNode *node = (DataNode*)(curNode.second);
                for(int i=0; i<node->numKeys; i++){
                    cout<<node->keys[i]<<" ";
                }
                cout<<endl;
            }
            else{
                IndexNode *node = (IndexNode*)(curNode.second);
                for(int i=0; i <= node->numKeys; i++){
                    q.push({++nodeID, node->ptrs[i]});
                    level.push(curLevel);
                }
                for(int i=0; i<node->numKeys; i++){
                    cout<<node->keys[i]<<" ";
                }
                cout<<endl;
            }
        }
        cout<<endl<<endl;
    }
};

int main()
{
    BPlusTree *tree = NULL;
    int key, choice;
    int numIndexPointers, numDataPointers;
    //node* searchResult;
    int searchResultIndex;
    do
    {
        cout << "1. Create new tree" << endl;
        cout << "2. Add new key to the tree" << endl;
        cout << "3. Search a key in the tree" << endl;
        cout << "4. Postorder traversal of the tree" << endl;
        cout << "Any other choice to exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        cout << "You entered " << choice << endl;

        switch (choice)
        {
        case 1:
            if (tree != NULL)
            {
                delete tree;
            }
            cout << "Enter number of Index pointers: ";
            cin >> numIndexPointers;
            cout << "Enter number of Data pointers: ";
            cin >> numDataPointers;
            // if (numIndexPointers % 2 == 0 || numDataPointers % 2)
            // {
            //     cout << "Note:: Index pointers must be in odd number and Data pointers must be in even number" << endl;
            //     exit(0);
            // }
            tree = new BPlusTree(numIndexPointers);
            break;

        case 2:
            if (tree == NULL)
            {
                cout << "Create a new tree first" << endl;
                break;
            }
            cout << "Enter key to add ";
            cin >> key;
            tree->insertmain(key);
            break;
        case 3:
            if (tree == NULL)
            {
                cout << "Create a new tree first" << endl;
                break;
            }
            cout << "Enter key to search: ";
            cin >> key;
            {
                DataNode *searchResult = tree->search(tree->root, key, tree->depth);
                if (searchResult == NULL)
                {
                    cout << "Key " << key << " not found." << endl;
                }
                else
                {
                    cout << "Key " << key << " found" << endl;
                }
            }
            break;
        case 4:
            if (tree == NULL)
            {
                cout << "Create a new tree first" << endl;
                break;
            }
            tree->LevelOrderTraversal(tree->root, tree->depth);
            break;

        default:
            return 0;
        }
    } while (true);
    return 0;
}