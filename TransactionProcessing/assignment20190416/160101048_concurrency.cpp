#include <bits/stdc++.h>
using namespace std;

/*this class contains declarations of all the data structures that are used to perform the given tasks*/

class Transactions{
public:
	void readInput();   // to read the given input file
	void PossibleChains();  // to find all possible chains
	void isRecoverable();  // to check whether schedule is recoverable or not
	void isCascadeless();  // to check whether schedule is cascadeless or not
    void isSerializable();
private:
	vector<pair<int, pair<char,int> > > transaction; 
	// it stores all the trasaction details
    unordered_map<int,int> commit;
    // it stores the position of commit operation of a trasaction 
    vector<pair<int,vector<int> > > chain;
    // to store all the possible chains, each chain represents collection of read operation on a dataitem that is written by a transaction before those read operations
    vector< pair<int,pair<int,int> > > firstReadArray; 
    // it represents a vector where each elements is a tuple containing tid of write operation, position of first read in a chain and its tid  
    set<int> tids;
    void dfs_visit(vector<vector<bool>> &adjmatrix, vector<int> &color, vector<int> &topolist, int u, bool &cycle);
    vector<int> commitOrder;
};

void Transactions::readInput(){
	/*
        -->this function read the given input file and store all the transactions details in d.s. 'transactions'
         
	*/
	int Tid, DataItem=0;
    char Opr;
    while(cin>>Tid){
        tids.insert(Tid);
        cin>>Opr;
        if(Opr!='C')
            cin>>DataItem;
        else{
            commit[Tid]=transaction.size();
            commitOrder.push_back(Tid);
        }
        // Not covering if input format is wrong.
        transaction.push_back({Tid,{Opr,DataItem}});
    }
}

void Transactions::PossibleChains(){
	/*
	    --> this method find all the possible chains and store it in d.s. 'chain'
	    --> each chain represents collection of read operations on a dataitem that is written by a transaction before those read operations
        
	*/
	vector<int> temp;
	pair<int,int> firstRead;
	for(int i=0; i<transaction.size(); i++){
        if(transaction[i].second.first!='W')
            continue;

        int DataItem=transaction[i].second.second;

        for(int j=i+1; j<transaction.size(); j++){
            if(transaction[j].second.second!=DataItem)
                continue;
            if(transaction[j].second.first=='R' && transaction[i].first != transaction[j].first){
            	if(temp.size()==0)
            		firstRead={transaction[j].first,j};
            	temp.push_back(transaction[j].first);
            }
            if(transaction[j].second.first=='W')
                break;
        }
        if(temp.size()){
            chain.push_back({transaction[i].first,temp});
            firstReadArray.push_back({transaction[i].first,{firstRead}});
        }
        temp.clear();
    }
}

void Transactions :: dfs_visit(vector<vector<bool>> &adjmatrix, vector<int> &color, vector<int> &topolist, int u, bool &cycle) {
    color[u] = 1; // 1- gray
    for (int v = 0; v < tids.size(); v++) {
        if (!adjmatrix[u][v]) continue;
        if (color[v] == 0) {
            dfs_visit(adjmatrix, color, topolist, v, cycle);
        }
        if (color[v] == 1) // back edge
            cycle = true;
    }
    color[u] = 2;
    topolist.push_back(u);
}   

void Transactions::isSerializable() {
    unordered_map<int, int> reversemap, map; // tid map to 0 to tids.size()-1
    int t = 0;
    set<int> :: iterator x;
    for (x = tids.begin(); x != tids.end(); x++) {
        reversemap[*x] = t;
        map[t] = *x;
        t++;
    }
    
    vector<vector<bool>> adjmatrix(tids.size(), vector<bool>(tids.size(), false));
    for (int i = 0; i < transaction.size(); i++) {
        for (int j = i+1; j < transaction.size(); j++) {
            int tid1 = transaction[i].first, tid2 = transaction[j].first;
            if (tid1 == tid2) // same transaction id
                continue;
            if (transaction[i].second.first == 'C' || transaction[j].second.first == 'C') // commit op are not needed for serializability
                continue;
            if (transaction[i].second.second != transaction[j].second.second) // different data item
                continue;
            if (transaction[i].second.first == 'R' && transaction[j].second.first == 'R') // read on same data item
                continue;

            adjmatrix[reversemap[tid1]][reversemap[tid2]] = true;
            // cout << tid1 << transaction[i].second.first << transaction[i].second.second << endl;
            // cout << tid2 << transaction[j].second.first << transaction[j].second.second << endl;             
            // cout << endl;
        }
    }
    // for (int i = 0; i < tids.size(); i++) {
    //     for (int j = 0; j < tids.size(); j++) {
    //         cout << adjmatrix[i][j] << " ";
    //     }
    //     cout << endl;
    // }
    // dfs
    vector<int> color(tids.size(), 0); // 0-white 
    vector<int> topolist; 
    bool cycle = false;
    for (int i = 0; i < tids.size(); i++) {
        if (color[i] == 0) {
            dfs_visit(adjmatrix, color, topolist, i, cycle);
        }
    }
    if (cycle) {
        cout << "Conflict serializable: No" << endl;
        cout << "Commit ordering: Not applicable" << endl;
    }
    else {
        cout << "Conflict serializable: Yes" << endl;
        // cout << "Equivalent serial order: " << endl;
        // reverse(topolist.begin(), topolist.end());
        // for (int i = 0; i < topolist.size(); i++) {
        //     cout << map[topolist[i]] << " ";
        // }
        // cout << endl;  

        // the commit order given in input must correspond to reverse of a possible topological order
        vector<int> candidate;
        for (int i = 0 ; i < tids.size(); i++) {
            candidate.push_back(reversemap[commitOrder[i]]);
        }
        // reverse(candidate.begin(), candidate.end()); // is this an topological order?
        // // for (int i = 0 ; i < tids.size(); i++) {
        // //     candidate.push_back(reversemap[commitOrder[i]]);
        // // }
        // assert(candidate.size()==tids.size());
        
        for (int i = 0; i < tids.size(); i++) {
            for (int j = i+1; j < tids.size(); j++) {
                if (adjmatrix[candidate[j]][candidate[i]]) {
                    cout << "Commit ordering: No" << endl;
                    return;
                }
            }
        }
        cout << "Commit ordering: Yes" << endl;
    }
}

void Transactions::isRecoverable(){
	/*
         --> this method checks whether the given schedule is recoverable or not
	*/
    // for (int i = 0; i < chain.size(); i++) {
    //     cout << "TID " << chain[i].first << ": ";
    //     for (int j = 0; j < chain[i].second.size(); j++) {
    //         cout << chain[i].second[j] << " ";
    //     }  
    //     cout << endl;
    // }
	for(int i=0; i<chain.size(); i++){
        int x = commit[chain[i].first];
        for(int j=0; j<chain[i].second.size(); j++){
            if(x>=commit[chain[i].second[j]]){
            	cout<<"No"<<endl;
            	cout<<"Transaction "<<chain[i].second[j]<<" is reading DataItem written by Transaction "<<chain[i].first<<endl;
            	cout<<"And Transaction "<<chain[i].first<<" commits after Transaction "<<chain[i].second[j]<<endl;
            	return;
            }
        }
    }
    cout<<"Yes"<<endl;
}

void Transactions::isCascadeless(){
	/*
         --> this method checks whether the given schedule is cascadeless or not
	*/
	for(int i=0; i<firstReadArray.size(); i++){
        int x = commit[firstReadArray[i].first];
        if(x>=firstReadArray[i].second.second){
        	cout<<"No"<<endl;
        	cout<<"Transaction "<<firstReadArray[i].second.first<<" is reading DataItem written by Transaction "<<firstReadArray[i].first<<endl;
        	cout<<"And Transaction "<<firstReadArray[i].first<<" commits after reading by Transaction "<<firstReadArray[i].second.first<<endl;
        	return;
        }
    }
    cout<<"Yes"<<endl;
}



int main() {
	Transactions obj;
	obj.readInput();   // to read input file

	obj.PossibleChains(); //to find all possible chains
	// obj.isRecoverable();  // to check whether schedule is recoverable or not
	// obj.isCascadeless(); // to check whether schedule is cascadeless or not 
    obj.isSerializable();

	return 0;
}
