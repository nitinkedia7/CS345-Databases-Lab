#include <bits/stdc++.h>
using namespace std;

class logEntry {
public:
    int Tid;
    char operation;
    char dataItem;
    int oldVal, newVal;

    // for start, commit, abort
    logEntry(int Tid, char operation) {
        this->Tid = Tid;
        this->operation = operation;
    }

    // for read
    logEntry(int Tid, char operation, char dataItem) {
        this->Tid = Tid;
        this->operation = operation;
        this->dataItem = dataItem;
    }

    // for write
    logEntry(int Tid, char operation, char dataItem, int oldVal, int newVal) {
        this->Tid = Tid;
        this->operation = operation;
        this->dataItem = dataItem;
        this->oldVal = oldVal;
        this->newVal = newVal;
    }
    // for undo
    logEntry(int Tid, char operation, char dataItem, int newVal) {
        this->Tid = Tid;
        this->operation = operation;
        this->dataItem = dataItem;
        this->newVal = newVal;
    }
};

void undo(vector<logEntry> &logFile, map<int, int> &undoList, map<char, int> &currentValues) {
    for (int i = logFile.size()-1; i >= 0; i--) {
        logEntry curEntry = logFile[i];
        if (curEntry.operation == 'W') {
            if (undoList[curEntry.Tid]) {
                currentValues[curEntry.dataItem] = curEntry.oldVal;
                logFile.push_back(logEntry(curEntry.Tid, 'U', curEntry.dataItem, curEntry.oldVal));
            }
        }
        else if (curEntry.operation == 'S') {
            if (undoList[curEntry.Tid]) {
                logFile.push_back(logEntry(curEntry.Tid, 'A'));
            }
        }
    }
    for (auto it : currentValues) {
        cout << it.first << ": " << it.second << endl;
    }
    cout << endl;
}

void print(vector<logEntry> logFile) {
    for (auto curEntry : logFile ) {
        if (curEntry.operation == 'S' || curEntry.operation == 'C' || curEntry.operation == 'A') {
            cout << curEntry.operation << " " << curEntry.Tid << endl;
        }
        else if (curEntry.operation == 'R') {
            cout << curEntry.operation << " " << curEntry.Tid << " " << curEntry.dataItem << endl;
        }
        else if (curEntry.operation == 'W') {
            cout << curEntry.operation << " " << curEntry.Tid << " " << curEntry.dataItem << " ";
            cout << curEntry.oldVal << " " << curEntry.newVal << endl;
        }
        else if (curEntry.operation == 'U') {
            cout << curEntry.operation << " " << curEntry.Tid << " " << curEntry.dataItem << " ";
            cout << curEntry.newVal << endl;
        }
        else {
            cout << "Invalid operation " << curEntry.operation << endl;
        }
    }
}

int main() {
    int dataItemCount;
    cin >> dataItemCount;
    map<char, int> currentValues;
    char dataItem;
    int dataItemValue;
    for (int i = 0; i < dataItemCount; i++) {
        cin >> dataItem >> dataItemValue;
        currentValues[dataItem] = dataItemValue;
    }
    for (auto it : currentValues) {
        cout << it.first << ": " << it.second << endl;
    }
    cout << endl;
    vector<logEntry> logFile;
    char operation;
    int Tid;
    int newVal;
    map<int, int> undoList;
    while (cin >> operation) {
        if (operation == 'S') {
            cin >> Tid;
            logFile.push_back(logEntry(Tid, operation));
            undoList.insert({Tid, 1});
        }
        else if (operation == 'C') {
            cin >> Tid;
            logFile.push_back(logEntry(Tid, operation));
            undoList[Tid] = 0; // committed or aborted transactions should not be rollback
        }
        else if (operation == 'R') {
            cin >> Tid;
            cin >> dataItem;
            // logFile.push_back(logEntry(Tid, operation, dataItem));
        }
        else if (operation == 'W') {
            cin >> Tid;
            cin >> dataItem;
            cin >> newVal;
            logFile.push_back(logEntry(Tid, operation, dataItem, currentValues[dataItem], newVal));
            currentValues[dataItem] = newVal;
        }
        else {
            cout << "Invalid operation" << endl;
        }
    }
    undo(logFile, undoList, currentValues);
    print(logFile);
    return 0;
}