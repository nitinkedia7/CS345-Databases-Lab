#include <bits/stdc++.h>
using namespace std;

class Bucket {
public:
    list<int> bucket;     
};

class LinearHashing {
    int next, level, bs, n;
    vector<Bucket*> dir;
public:
    void createHashTable(int n, int bs) {
        this->next = 0;
        this->n = n;
        this->level = 0;
        this->bs = bs;
        this->dir.resize(n);
        for (int i = 0; i < n; i++) {
            this->dir[i] = new Bucket();
        } 
    }
    void insertElement(int v) {
        int powlevel = pow(2, this->level) * this->n; 
        int bi = v % powlevel;
        if (bi < next) { // splitted, see level+1
            bi = v % (2 * powlevel); 
        } 
        if (this->dir[bi]->bucket.size() < this->bs) {
            this->dir[bi]->bucket.push_back(v);
        }
        else { // need to add to overflow, split next, wrap next
            this->dir[bi]->bucket.push_back(v);
            if (this->dir[bi]->bucket.size() % this->bs == 1)
                splitBucket();
        }
    }
    void splitBucket() { // split next, wrap next
        int powlevel = pow(2, this->level) * this->n;
        int fb = this->next;
        int sb = fb + powlevel;
        this->dir.push_back(new Bucket());
        // this->dir[sb] = new Bucket();
        list<int> &fbucket = this->dir[fb]->bucket;
        list<int> &sbucket = this->dir[sb]->bucket;
        for (auto itr = fbucket.begin(); itr != fbucket.end();) {
            // if (*itr & powlevel) {
            if (*itr % (2*powlevel) == sb) {
                sbucket.push_back(*itr);
                itr = fbucket.erase(itr);
            }
            else itr++;
        }

        this->next += 1;
        if (this->next == powlevel) {
            this->next = 0;
            this->level++;
            // this->dir.resize(2*this->dir.size());
        }
    }
    bool searchElement(int v) {
        int powlevel = pow(2, this->level) * this->n; 
        int bi = v % powlevel;
        if (bi < next) { // splitted, see level+1
            bi = v % (2 * powlevel); 
        }
        list<int> &bucket = this->dir[bi]->bucket;
        for (auto itr = bucket.begin(); itr != bucket.end(); itr++) {
            if (*itr == v) return true;
        }
        return false; 
    }
    bool deleteElement(int v) {
        int powlevel = pow(2, this->level) * this->n; 
        int bi = v % powlevel;
        if (bi < next) { // splitted, see level+1
            bi = v % (2 * powlevel); 
        }
        list<int> &bucket = this->dir[bi]->bucket;
        for (auto itr = bucket.begin(); itr != bucket.end(); itr++) {
            if (*itr == v) {
                bucket.erase(itr);
                return true;
            }
        }
        return false; 
    } 
    void printHashTable() {
        cout<<"\n ***** Hash Table ***** \n"<<endl;
	    cout<<"level :: " << this->level << endl;
	    cout<<"next :: " << this->next << endl;
        cout<<"bucketIndex :: | Elements |"<<endl;
        int powlevel = pow(2, this->level) * this->n; 
        for (int i = 0; i < (this->next + powlevel); i++) {
            cout << i << " :: | ";
            list<int> &bucket = this->dir[i]->bucket;
            for (auto itr = bucket.begin(); itr != bucket.end(); itr++) {
                cout << *itr << " ";
            }
            cout << "|" << endl;
        } 
    }
};

int main() {
    int choice;
    LinearHashing *d=NULL;
    do{
        cout<<"1. Create new LinearHashTable"<<endl;
        cout<<"2. Add an element to the HashTable"<<endl;
        cout<<"3. Search an element to the HashTable"<<endl;
        cout<<"4. Delete an element from the HashTable"<<endl;
        cout<<"5. Print HashTable"<<endl;

        cout<<"Any other choice to exit"<<endl;
        cout<<"Enter your choice: ";
        cin>>choice;
        cout<<"You entered "<<choice<<endl;
        switch(choice){
            case 1:
                if(d!=NULL)
                    delete d;
                d= new LinearHashing();
                int globalDepth;
                cout<<"Enter number of buckets: ";
                cin>>globalDepth;
                int bucketSize;
                cout<<"Enter number of entries in bucket: ";
                cin>>bucketSize;
                d->createHashTable(globalDepth,bucketSize);
                break;
            case 2:
                int add;
                cout<<"Enter element to add: ";
                cin>>add;
                d->insertElement(add);
                break;
            case 3:
                int search;
                cout<<"Enter element to search: ";
                cin>>search;
                if(d->searchElement(search))
                    cout<<"Element is found"<<endl;
                else
                    cout<<"Element is not found"<<endl;
                break;
            case 4:
                int del;
                cout<<"Enter element to delete: ";
                cin>>del;
                if(d->deleteElement(del))
                    cout<<"Element is deleted"<<endl;
                else
                    cout<<"Element is not found"<<endl;
                break;
            case 5:
                d->printHashTable();
                break;
            default:
                return 0;
        }
    }while(true);
}