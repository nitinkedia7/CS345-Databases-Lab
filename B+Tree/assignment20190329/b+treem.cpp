#include <bits/stdc++.h>
using namespace std;

int indexNodeCount = 0, dataNodeCount = 0;

class Node {
public:
    int numKeys;
    int t;
    int *keys;
    void *parent;
    int index;
    
    Node(int t) {
        this->t = t;
        this->numKeys = 0;
        this->parent = NULL;
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
        this->index = indexNodeCount;
        indexNodeCount++;
    }
    void InsertKey(int key, void *newChild) {
        if (this->numKeys == 0) {
            // assert(1==2);
            // this->InsertKey(key, newChild);
            this->keys[0] = key;
            this->ptrs[1] = newChild;
            this->numKeys++;
        }
        else {
            this->ptrs[this->numKeys+1] = this->ptrs[this->numKeys]; // shift rightmost ptr
            for (int i = this->numKeys-1; i >= 0; i--) {
                if (this->keys[i] > key) {
                    this->keys[i+1] = this->keys[i]; // shift to make room for key
                    this->ptrs[i+1] = this->ptrs[i]; // shift to make room for ptr
                }
                if (this->keys[i] <= key)  {
                    this->keys[i+1] = key; // place key
                    this->ptrs[i+2] = newChild; // place ptr
                    break;
                }
                if (i == 0) {
                    this->keys[i] = key;
                    this->ptrs[i+1] = newChild; 
                    break;
                }
            }
        }
        this->numKeys++;
        return;
    }
};

class DataNode : public Node {
public:
    DataNode *left, *right;
    DataNode(int t) : Node(t) {
        keys = new int[2*t+1];
        left = this;
        right = this;
        this->index = dataNodeCount;
        dataNodeCount++;
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
    int t_index, t_data;
    void * root;
    BPlusTree(int t_index, int t_data) {
        this->depth = 0;
        this->t_index = t_index;
        this->t_data = t_data;
        this->root = new DataNode(t_data);
        indexNodeCount = 0;
        dataNodeCount = 0;
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
                    return search(curNode->ptrs[i], key, depth-1);
                }
            }
            return search(curNode->ptrs[curNode->numKeys], key, depth-1);
        }
    }
    void insertmain(int key) {
        int up1 = INT_MAX, up2 = INT_MAX;
        void *d1 = NULL;
        void *d2 = NULL;
        insert(this->root, key, this->depth, &up1, &up2, &d1, &d2);
        if (d1 == NULL) {
            return;
        }
        else if (d2 == NULL) {
            // make a new root;
            IndexNode *newRoot = new IndexNode(this->t_index);
            newRoot->numKeys++;
            newRoot->keys[0] = up1;
            newRoot->ptrs[0] = this->root;
            newRoot->ptrs[1] = d1;
            this->root = newRoot;
            this->depth++;
        }
        else {
            if (this->t_index > 1) {
                IndexNode *newRoot = new IndexNode(this->t_index);
                newRoot->numKeys = 2;
                newRoot->keys[0] = up1;
                newRoot->keys[1] = up2;
                newRoot->ptrs[0] = this->root;
                newRoot->ptrs[1] = d1;
                newRoot->ptrs[2] = d2;
                this->root = newRoot;
                this->depth++;
            }
            else {
                IndexNode *newRoot = new IndexNode(this->t_index);
                newRoot->numKeys++;
                newRoot->keys[0] = up1;
                newRoot->ptrs[0] = this->root;
                newRoot->ptrs[1] = d1;
                this->root = newRoot;
                this->depth++;
                this->splitIndexNode((IndexNode*)this->root, up2, d2, &up1, (IndexNode**)&d1);
                IndexNode *newRoot1 = new IndexNode(this->t_index);
                newRoot1->numKeys++;
                newRoot1->keys[0] = up1;
                newRoot1->ptrs[0] = this->root;
                newRoot1->ptrs[1] = d1;
                this->root = newRoot1;
                this->depth++;
            }
        }
        return;
    };
    void insert(void *node, int key, int depth, int *up1, int *up2, void **d1, void **d2) {
        if (depth == 0) { // reached datanode, attempt insert
            DataNode *curNode = (DataNode *) node; 
            if (curNode->numKeys < 2 * curNode->t) { // insert to nonfull datanode 
                curNode->InsertKey(key);
                return; // if no split return with d1 = d2 = NULL;
            }
            else { // threeway split leaf datanode 
                // DataNode *d1 = NULL, *d2 = NULL;
                // int up1, up2;
                // this->splitDataNode(curNode, key, &up1, &up2, &d1, &d2);
                this->splitDataNode(curNode, key, up1, up2, (DataNode **) d1, (DataNode **) d2);
                // split successful now set base for backpropagation
                // can have potentially two new separators to backpropagate
                return;
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
            insert(nextNode, key, depth-1, up1, up2, d1, d2);
            if (*d1 == NULL) {
                return;
            }
            else if (*d2 == NULL) { 
                if (curNode->numKeys < 2*curNode->t-1) {
                    curNode->InsertKey(*up1, *d1); // check newChild replaced by d1
                    *d1 = NULL;
                    return; 
                }
                else { // need to twoway split curNode which is an indexnode
                    this->splitIndexNode(curNode, *up1, *d1, up1, (IndexNode **) d1);
                    assert(*d1 != NULL);
                    *d2 = NULL;
                    return;
                }
            }
            else {
                if (curNode->numKeys < 2*curNode->t-2) {
                    curNode->InsertKey(*up1, *d1); // check newChild replaced by d1
                    curNode->InsertKey(*up2, *d2); // check newChild replaced by d1
                    *d1 = NULL;
                    *d2 = NULL;
                    return; 
                }
                else if (curNode->numKeys == 2*curNode->t-2) {
                    curNode->InsertKey(*up1, *d1);
                    this->splitIndexNode(curNode, *up2, *d2, up1, (IndexNode **) d1);
                    *d2 = NULL;
                    return;
                }
                else {
                    this->splitIndexNode(curNode, *up1, *d1, up1, (IndexNode **) d1);
                    ((IndexNode *) *d1)->InsertKey(*up2, *d2);
                    *d2 = NULL;
                    return;
                }
            }
        }
    }
    void splitDataNode(DataNode *d0, int key, int *up1, int *up2, DataNode **d1, DataNode **d2) {
        int t = d0->t;
        d0->InsertKey(key);
        assert(d0->numKeys == 2*t+1);
        *d1 = new DataNode(t);
        *d2 = new DataNode(t);

        int a, b, c;
        a = b = c = (d0->numKeys)/3;
        if ((d0->numKeys) %3 == 1) a += 1;
        else if ((d0->numKeys) %3 == 2) {
            a += 1;
            b += 1;
        }
        for (int i = a; i < a+b; i++) {
            (*d1)->keys[i-a] = d0->keys[i];
        }
        *up1 = d0->keys[a-1] + 1;
        for (int i = a+b; i < a+b+c; i++) {
            (*d2)->keys[i-a-b] = d0->keys[i];
        }
        *up2 = d0->keys[a+b-1] + 1;

        d0->numKeys = a;
        (*d1)->numKeys = b;
        (*d2)->numKeys = c;
        return;
    }
    void splitIndexNode(IndexNode *node, int key, void *newChild, int *up, IndexNode **pnewNonLeaf) {
        int t = node->t;
        node->InsertKey(key, newChild); // forcefully insert extra key since to make select middle easy
        // assert(node->numKeys == 2*t);

        *pnewNonLeaf = new IndexNode(t);
        *up = node->keys[t];

        (*pnewNonLeaf)->ptrs[t-1] = node->ptrs[2*t]; 
        for (int i = 2*t-1; i > t; i--) {
            (*pnewNonLeaf)->keys[i-t-1] = node->keys[i];
            (*pnewNonLeaf)->ptrs[i-t-1] = node->ptrs[i];
        }
        node->numKeys = t;
        (*pnewNonLeaf)->numKeys = t-1;
        (*pnewNonLeaf)->parent = node->parent;
        return;
    }
    void LevelOrderTraversal(void *root, int depth) {
        queue<pair<int,void *> > q;
        queue<int> level;
        queue<int> parent;
        int nodeID = 0, p = -1;
        pair<int, void*> curNode;
        int curLevel=0,prevLevel=-1;
        depth = depth + 1;
        q.push({nodeID, root});
        parent.push(p);
        level.push(0);
        cout<<"\n*** NodeID :: Level :: Values ***\n"<<endl;
        while(!q.empty()){
            curNode = q.front(); q.pop();
            p = parent.front(); parent.pop();

            if(prevLevel != level.front()) {
                depth = depth-1;
                prevLevel = curLevel++;
            }
            level.pop();

            // cout<<curNode.first<< " :: " << curLevel << " :: ";
            if(depth==0){
                DataNode *node = (DataNode*)(curNode.second);
                cout << "Data node " << curNode.first << ": ";
                cout << "Parent: Index Node " << p << "\t";
                cout << "Contents: ";                
                for(int i=0; i<node->numKeys; i++){
                    cout<<node->keys[i]<<" ";
                }
                cout<<endl;
            }
            else{
                IndexNode *node = (IndexNode*)(curNode.second);
                cout << "Index node " << curNode.first << ": ";
                cout << "Parent: Index Node " << p << "\t";
                int pc = curNode.first;
                cout << "Sequence of keys and children: ";
                for(int i=0; i < node->numKeys; i++){
                    if (depth > 1) {
                        q.push({++nodeID, node->ptrs[i]});
                        cout << "Index Node " << nodeID << ", ";
                    }
                    else {
                        q.push({++nodeID, node->ptrs[i]});
                        cout << "Data Node " << nodeID << ", ";
                    } 
                    cout<<node->keys[i]<<", ";
                    level.push(curLevel);
                    parent.push(pc);
                }
                if (depth > 1) {
                        q.push({++nodeID, node->ptrs[node->numKeys]});
                        cout << "Index Node " << nodeID;
                        level.push(curLevel);
                        parent.push(pc);
                    }
                else {
                        q.push({++nodeID, node->ptrs[node->numKeys]});
                        cout << "Data Node " << nodeID;
                        level.push(curLevel);
                        parent.push(pc);
                    } 
                cout<<endl;
            }
        }
        cout<<endl;
    }
    // void LevelOrderTraversal(void *root, int depth) {
    //     queue<pair<int,void *> > q;
    //     queue<int> level;
    //     queue<int> parent;
    //     int nodeID = 0, p = -1;
    //     pair<int, void*> curNode;
    //     int curLevel=0,prevLevel=-1;
    //     depth = depth + 1;
    //     q.push({nodeID, root});
    //     parent.push(p);
    //     level.push(0);
    //     cout<<"\n*** NodeID :: Level :: Values ***\n"<<endl;
    //     while(!q.empty()){
    //         curNode = q.front(); q.pop();
    //         p = parent.front(); parent.pop();

    //         if(prevLevel != level.front()) {
    //             depth = depth-1;
    //             prevLevel = curLevel++;
    //         }
    //         level.pop();

    //         // cout<<curNode.first<< " :: " << curLevel << " :: ";
    //         if(depth==0){
    //             DataNode *node = (DataNode*)(curNode.second);
    //             cout << "Data node " << node->index << ": ";
    //             cout << "Parent: Index Node " << p << "\t";
    //             cout << "Contents: ";                
    //             for(int i=0; i<node->numKeys; i++){
    //                 cout<<node->keys[i]<<" ";
    //             }
    //             cout<<endl;
    //         }
    //         else{
    //             IndexNode *node = (IndexNode*)(curNode.second);
    //             cout << "Index node " << node->index << ": ";
    //             cout << "Parent: Index Node " << p << "\t";
    //             int pc = node->index;
    //             cout << "Sequence of keys and children: ";
    //             for(int i=0; i < node->numKeys; i++){
    //                 if (depth > 1) {
    //                     q.push({++nodeID, node->ptrs[i]});
    //                     cout << "Index Node " << ((IndexNode*)node->ptrs[i])->index << ", ";
    //                 }
    //                 else {
    //                     q.push({++nodeID, node->ptrs[i]});
    //                     cout << "Data Node " << ((DataNode*)node->ptrs[i])->index << ", ";
    //                 } 
    //                 cout<<node->keys[i]<<", ";
    //                 level.push(curLevel);
    //                 parent.push(pc);
    //             }
    //             if (depth > 1) {
    //                     q.push({++nodeID, node->ptrs[node->numKeys]});
    //                     cout << "Index Node " << ((IndexNode*)node->ptrs[node->numKeys])->index;
    //                     level.push(curLevel);
    //                     parent.push(pc);
    //                 }
    //             else {
    //                     q.push({++nodeID, node->ptrs[node->numKeys]});
    //                     cout << "Data Node " << ((DataNode*)node->ptrs[node->numKeys])->index;
    //                     level.push(curLevel);
    //                     parent.push(pc);
    //                 } 
    //             cout<<endl;
    //         }
    //     }
    //     cout<<endl;
    // }
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
            if (numIndexPointers % 2 == 0 || numDataPointers % 2)
            {
                cout << "Note:: Index pointers must be in odd number and Data pointers must be in even number" << endl;
                exit(0);
            }
            tree = new BPlusTree((numIndexPointers+1)/2, numDataPointers/2);
            break;

        case 2:
            if (tree == NULL)
            {
                cout << "Create a new tree first" << endl;
                break;
            }
            cout << "Enter key to add: ";
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