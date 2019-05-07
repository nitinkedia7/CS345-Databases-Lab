#include <bits/stdc++.h>
using namespace std;

class entry {
public:
    int readTid;
    int writeTid; // last tid which has written
    int dataItem;
    int count;
    entry() {
        this->readTid = -1;
        this->writeTid = -1;
        this->dataItem = -1;
        this->count = -1;
    }
};

bool isSameEntry(entry &e1, entry &e2) {
    if (e1.readTid == -1 || e1.dataItem == -1) return false;
    if (e2.readTid == -1 || e2.dataItem == -1) return false;
    
    bool cond1 = (e1.readTid == e2.readTid);
    bool cond2 = (e1.writeTid == e2.writeTid);
    bool cond3 = (e1.dataItem == e2.dataItem);
    bool cond4 = (e1.count == e2.count);

    if (cond1 && cond2 && cond3 && cond4) return true;
    return false;
}

bool isSame(vector<entry> &view1, vector<entry> &view2) {
    for (auto v1 : view1) {
        bool cond = false;
        for (auto v2 : view2) {
            cond = (cond || isSameEntry(v1, v2)); 
        }
        if (cond == false) return false;
    }
    return true;
}

void scan(int n, vector<entry> &view, map<int, int> &firstRead, map<int, int> &lastWrite) {
    char operation;
    int Tid;
    int dataItem;
    map<pair<int, int>, int> writeTime;
    int i = 0;
    while (i < n) {
        cin >> Tid;
        cin >> operation;
        i += 2;
        if (operation == 'R') {
            cin >> dataItem;
            i += 1;
            // cout << Tid << ", " << operation << ", " << dataItem << endl;
            // maintain first read for given data item
            if (firstRead.find(dataItem) == firstRead.end()) {
                firstRead[dataItem] = Tid;
            }
            // store the last tid which wrote this dataItem
            entry *e = new entry();
            e->readTid = Tid;
            e->dataItem = dataItem;
            e->writeTid = lastWrite[dataItem];
            e->count = writeTime[make_pair(lastWrite[dataItem], dataItem)];
            view.push_back(*e);
        }
        else if (operation == 'W') {
            cin >> dataItem;
            i += 1;
            // cout << Tid << ", " << operation << ", " << dataItem << endl;
            // last read for a given dataItem;
            lastWrite[dataItem] = Tid;
            // maintain the number of times a Tid has written a dataItem till now
            pair<int, int> p = make_pair(Tid, dataItem);
            writeTime[p] = writeTime[p]+1;
        }
        else if (operation == 'C'){
            // cout << Tid << ", " << operation << endl;
            continue;
        }
        else {
            cout << operation << endl;
            cout << "Invalid operation" << endl;
        }
    }
    return;
}

void print(map<int, int> &map1) {
    for (auto m : map1) {
        cout << m.first << " " << m.second << endl;
    }
    cout << endl;
}

void printView(vector<entry> &view) {
    for (auto v : view) {
        cout << v.readTid << ", " << v.dataItem << ", ";
        cout << v.writeTid << ", " << v.count << endl;
    }
    cout << endl;
}

int main() {
    char operation;
    int Tid;
    int dataItem;
    int n;
    cin >> n;

    map<int, int> firstRead1, lastWrite1;
    vector<entry> view1;
    scan(n, view1, firstRead1, lastWrite1);

    // print(firstRead1);
    // print(lastWrite1);
    // printView(view1);

    map<int, int> firstRead2, lastWrite2;
    vector<entry> view2;
    scan(n, view2, firstRead2, lastWrite2);

    // print(firstRead2);
    // print(lastWrite2);
    // printView(view2);


    bool cond1 = (firstRead1 == firstRead2);
    bool cond2 = (lastWrite1 == lastWrite2);
    bool cond3 = isSame(view1, view2);
    bool cond4 = isSame(view2, view1);

    cout << "View equivalent: ";
    if (cond1 && cond2 && cond3 && cond4) {
        cout << "Yes" << endl;
    }
    else {
        cout << "No" << endl;
    }
}