#include <bits/stdc++.h>
using namespace std;

class Bucket {
public:
    int ld;
    list<int> bucket;
    Bucket(int ld) {
        this->ld = ld;
    }
};

class ExtendibleHashing {
    int gd, bs;
    vector<Bucket*> dir;
public:
    void createHashTable(int gd, int bs) {
        int buckets = pow(2, gd);
        this->gd = gd;
        this->bs = bs;
        this->dir.resize(buckets);
        for (int i = 0; i < buckets; i++) {
            this->dir[i] = new Bucket(gd);
        }
    }
    void insertElement(int v) {
        int buckets = pow(2, gd);
        int bi = v % buckets;
        if (this->dir[bi]->bucket.size() < this->bs) {
            this->dir[bi]->bucket.push_back(v);
            return;
        } 
        else {
            if (this->dir[bi]->ld == this->gd) { 
                if (this->dir[bi]->bucket.size() > this->bs) {// scenario 2.1
                    this->dir[bi]->bucket.push_back(v);
                }
                else { // 2.2
                    bool flag = true;
                    for (int i = 0; i < this->dir.size(); i++) {
                        assert(this->dir[i]->ld == this->gd);
                        if (i != bi) {
                            if (this->dir[i]->bucket.size() <= this->bs) {
                                flag = false;
                            }
                        }
                    }
                    // cout << "all overflow " << flag << endl;
                    if (flag) { // push v then double then split all /* 2.2.2 */
                        this->dir[bi]->bucket.push_back(v);
                        int oldSize = this->dir.size();
                        this->doubleHashTable();
                        for (int i = 0; i < oldSize; i++) {
                            this->splitBucket(i);
                        }
                    }
                    else { //2.2.1
                        this->dir[bi]->bucket.push_back(v);                
                    }
                }
            }
            else { // screnario 1 ld < gd
                this->splitBucket(bi);
                this->insertElement(v);
            }
            return;
        }
    }
    void doubleHashTable() {
        int oldSize = pow(2, gd);
        this->dir.resize(2*oldSize);
        for (int i = oldSize; i < 2*oldSize; i++) this->dir[i] = this->dir[i-oldSize];
        this->gd++;
    }
    void splitBucket(int bi) {
        int ld = this->dir[bi]->ld;
        int powld = pow(2, ld);
        int fbi = bi % powld;
        int sbi = fbi + powld;
        this->dir[sbi] = new Bucket(ld+1);
        this->dir[fbi]->ld++;
        list<int> &fbucket = this->dir[fbi]->bucket;
        list<int> &sbucket = this->dir[sbi]->bucket;
        for (auto itr = fbucket.begin(); itr != fbucket.end();) {
            if (*itr & powld) {
                sbucket.push_back(*itr);
                itr = fbucket.erase(itr);
            }
            else itr++;
        }
        powld = 2*powld;
        for (int i = sbi; i < this->dir.size(); i += powld) {
            this->dir[i] = this->dir[sbi];
        }
    }
    bool searchElement(int v) {
        int bi = v % (int)pow(2, this->gd);
        list<int> &bucket = this->dir[bi]->bucket;
        for (auto itr = bucket.begin(); itr != bucket.end(); itr++) {
            if (*itr == v) return true;
        }
        return false;
    }
    bool deleteElement(int v) {
        int bi = v % (int)pow(2, this->gd);
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
	    cout<<"globalDepth :: " << this->gd<<endl;
        cout<<"bucketIndex :: localDepth ::| Elements |"<<endl;
        for (int i = 0; i < this->dir.size(); i++) {
            int powld = pow(2, this->dir[i]->ld);
            if (i % powld == i) {
                cout << i << " :: | " << this->dir[i]->ld << " :: |";
                list<int> &bucket = this->dir[i]->bucket;
                for (auto itr = bucket.begin(); itr != bucket.end(); itr++) {
                    cout << *itr << " ";
                }
                cout << "|\n";
            } 
            else {
                cout << i << " :: | " << this->dir[i]->ld << " :: points to " << i%powld;
                cout << endl; 
            }
        }
    }
    void printHashTable2() {
        cout<<"\n ***** Hash Table ***** \n"<<endl;
	    cout<<"globalDepth :: " << this->gd<<endl;
        cout<<"bucketIndex :: | Elements |"<<endl;
        for (int i = 0; i < this->dir.size(); i++) {
            int powld = pow(2, this->dir[i]->ld);
            if (i % powld == i) {
                cout << i << " :: | ";
                list<int> &bucket = this->dir[i]->bucket;
                for (auto itr = bucket.begin(); itr != bucket.end(); itr++) {
                    cout << *itr << " ";
                }
                cout << "|\n";
            } 
            else {
                cout << i << " :: points to " << i%powld;
                cout << endl; 
            }
        }
    }
}; 

int main() {
    int choice;
    ExtendibleHashing *d=NULL;
    do{
        cout<<"1. Create new ExtendibleHashTable"<<endl;
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
                d= new ExtendibleHashing();
                int globalDepth;
                cout<<"Enter global depth: ";
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
                d->printHashTable2();
                break;
            default:
                return 0;
        }
    }while(true);
}