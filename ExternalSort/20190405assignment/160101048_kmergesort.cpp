// #include <iostream>
// #include <cmath>
// #include <vector>
// #include <algorithm>
// #include <climits>
// #include <cassert>
#include <bits/stdc++.h>

using namespace std;
int DISK_PAGE_SIZE;
int MEM_FRAME_SIZE;

class Page{
public:
	vector<int> arr;
	int validEntries;
	//initializes an empty page with invalid entries i.e. -1
	Page(){
		arr.resize(DISK_PAGE_SIZE, -1);
		validEntries = 0;
	}

	void writePage();
	void fillPage(vector<int> &v);
};

//writes page to cout
void Page :: writePage(){

	for(int i = 0; i < DISK_PAGE_SIZE; i++){
		if(this->arr[i] == -1)
			break;
		cout << this->arr[i] << " ";
	}
	cout << endl;
}

//fills page with contents from vector v
void Page :: fillPage(vector<int> &v){
	
	if(v.size() < DISK_PAGE_SIZE){
		cout << "vector should contain atleast as many entries as a page" << endl;
		exit(1);
	}
	int i = 0;
	this->validEntries = 0;
	for(i; i < v.size() && i < DISK_PAGE_SIZE; i++){
		if(v[i] == -1){
			this->validEntries = i;
			// break;
		}
		this->arr[i] = v[i];
	}
	// for (int i = this->validEntries; i < v.size() && i < DISK_PAGE_SIZE; i++) {
	// 	this->arr[i] = -1;
	// }
}


class Frame{
public:
	vector<int> arr;
	int validEntries;
	//initializes an empty page with invalid entries i.e. -1
	Frame()
	{
		arr.resize(MEM_FRAME_SIZE, -1);
		/*for(int i  = 0; i < MEM_FRAME_SIZE; i++)
			arr[i] = -1;*/
		validEntries = 0;
	}
	
	//fill Frame with given vector
	void fillFrame(vector<int> &v);
	void printFrame();
};

//fills frame with data from vector v
void Frame :: fillFrame(vector<int> &v){
	int i = 0;
	bool flag = false;
	while(i < v.size() && i < MEM_FRAME_SIZE){
		this->arr[i] = v[i];
		if(v[i] == -1){
			flag = true;
			this->validEntries = i;
			i++;
			break;
		}
		i++;
	}
	while(i < MEM_FRAME_SIZE){
		this->arr[i] = -1;
		i++;
	}
	if(!flag)
		this->validEntries = MEM_FRAME_SIZE;
}

//Prints all valid entries of a frame
void Frame :: printFrame()
{
	for(int i = 0; i < this->validEntries; i++)
	{
		cout << this->arr[i] << endl;
	}
}

class DiskFile{

public:
	vector<Page> data;
	int totalPages;
	int size; //in terms of number of entries
	
	DiskFile(){
		totalPages = 0;
		size = 0;
	}
	
	DiskFile(int s){
		size = 0;
		totalPages = s;
		data.resize(totalPages);
	}


	void readDiskFile();
	void writeDiskFile();
	void DiskFileCopy( DiskFile &f, int startPage, int endPage);
};

//creates DiskFile by reading input file through cin
void DiskFile :: readDiskFile(){
	
	int i = 0;
	this->size  = 0;
	this->totalPages = 0; 
	Page t;
	vector<int> temp(DISK_PAGE_SIZE,-1);
	bool flag = true;
	int x;
	int n;
	cin >> n;
	while(n > 0){
		cin >> x;
		flag = false;
		temp[i++] = x;
		this->size++;
		
		if(i == DISK_PAGE_SIZE){
			t.fillPage(temp);
			this->data.push_back(t);
			flag = true;
			this->totalPages++;
			i = 0;
		}
		n--;
	}

	if(!flag){
		
		while(i != DISK_PAGE_SIZE){
			temp[i++] = -1;	
		}
		t.fillPage(temp);
		this->data.push_back(t);
		this->totalPages++;
	}
}

//writes disk file to console
void DiskFile :: writeDiskFile(){
	cout << "Contents of Disk File: " <<endl;
	for(int  i = 0; i < totalPages; i++){
		cout << "Page: " << i << ": ";
		this->data[i].writePage();
	}
}

//copies contents of disk file f into destination disk file from startPage to endPage
void DiskFile :: DiskFileCopy(DiskFile &f, int startPage, int endPage){
	int j = 0; //start of source file
	int i = startPage; //start of destination file

	while(i <= endPage && j < f.totalPages && i < this->totalPages){
		this->data[i].validEntries = f.data[j].validEntries;
		for(int k = 0; k < DISK_PAGE_SIZE; k++){
			this->data[i].arr[k] = f.data[j].arr[k];
		}
		i++;
		j++;
	}
}

class MainMemory{

public:
	vector<Frame> data;
	int totalFrames;
	vector<bool> valid;

	MainMemory(int s){
		totalFrames = s;
		data.resize(s);
		valid.resize(s);
		for(int i = 0; i < s; i++)
			valid[i] = false;
		cout << "Mem created" << endl;
	}

	int loadPage(DiskFile &f, int n);
	int getEmptyFrame();
	int getValidEntries(int f);
	int getVal(int f, int i);
	void setVal(int f, int i, int val);
	void writeFrame(DiskFile &inputFile, int f, int p);
	void freeFrame(int f);
};

//loads nth page of disk file f to an empty frame if available
int MainMemory :: loadPage(DiskFile &f, int n){

	if(n >= f.totalPages)
		return -1; // invalid page

	for(int i = 0; i < this->totalFrames; i++){
		if(!this->valid[i]){
			this->data[i].fillFrame(f.data[n].arr);
			this->valid[i] = true;
			return i;
		}
	}
	return -1; //no empty frame is available
}

//returns an empty frame if available
int MainMemory :: getEmptyFrame(){

	vector<int> v(MEM_FRAME_SIZE, -1);

	for(int i = 0; i < this->totalFrames; i++){
		if(!this->valid[i]){
			this->data[i].fillFrame(v);
			this->valid[i] = true;
			return i;
		}
	}
	return -1; //no empty frame is available

}

//returns number of valid entries in frame f
int MainMemory :: getValidEntries(int f){
	if(f >= this->totalFrames || !this->valid[f])
		return -1;
	return this->data[f].validEntries;
}


// returns value stored at location i in frame f
int MainMemory :: getVal(int f, int i){
	if(!this->valid[f] || i >= MEM_FRAME_SIZE)
		return -1;
	return this->data[f].arr[i];
}

//assigns value val to ith location of frame f
void MainMemory :: setVal(int f, int i, int val){
	if(!this->valid[f] || i >= MEM_FRAME_SIZE){
		cout << "accessing invalid address" << endl;
		exit(1);
	}

	this->data[f].arr[i] = val;
	if(this->data[f].validEntries == i)
		this->data[f].validEntries = i+1;

}

//write Frame f to file at page p
void MainMemory :: writeFrame(DiskFile &inputFile, int f, int p){
	inputFile.data[p].validEntries = getValidEntries(f);

	for(int i = 0; i <	MEM_FRAME_SIZE; i++){
		inputFile.data[p].arr[i] = this->data[f].arr[i];
	}	
}

//clears frame f
void MainMemory :: freeFrame(int f){
	if(f < totalFrames)
		this->valid[f] = false;
}	

class ExtMergeSort{
public:
	int runSize; // size of run in terms of number of pages
	int totalPass; // number of passes performed
	int totalRuns;
	int k;
	// int b;
	vector<DiskFile> preserve;

	ExtMergeSort(int k){
		runSize = 0;
		totalPass = 0;
		totalRuns = -1;
		this->k = k;
		// this->preserve.resize(10);
	}

	void firstPass(DiskFile &inputFile, MainMemory &memory);
	void merge(DiskFile &inputFile, MainMemory &memory, int leftStart, int runSize, int k);
	void kWaySort(DiskFile &inputFile, MainMemory &memory);
	void print(int pass, int page);
};

void ExtMergeSort :: print(int pass, int page) {
	pass--, page--;
	if (pass < 0 || pass >= this->totalPass) {
		cout << "Pass " << pass+1 << ", Page " << page+1 << ": Invalid Pass number" << endl;
		return;
	}
	int t = this->preserve[pass].totalPages;
	if (page < 0 || page >= t) {
		cout << "Pass " << pass+1 << ", Page " << page+1 << ": Invalid Page number" << endl;
		return;		
	}
	int l = this->preserve[pass].data[page].validEntries;
	cout << "Pass " << pass+1 << ", Page " << page+1 << ": ";
	for (int i = 0; i < l; i++) {
		cout << this->preserve[pass].data[page].arr[i] << " ";
	}
	cout << endl;
}

//creates initial runs of B page size
void ExtMergeSort :: firstPass(DiskFile &inputFile, MainMemory &memory){
	int c;
    int b = memory.totalFrames;
	for (int i = 0; i < inputFile.totalPages; i += b) {
		c = 0;
		int r = min(inputFile.totalPages, i+b); // [i,r)
		vector<int> frames(b, -1);
		for (int j = i; j < r; j++) {
			frames[j-i] = memory.loadPage(inputFile, j);
			c += memory.getValidEntries(frames[j-i]);
		}
		// apply bubblesort to B pages
		// cout << c << endl;
		for (int j = 0; j < c; j++) {
			for (int k = 1; k < c; k++) { // k-1, k
 				int cp = k/DISK_PAGE_SIZE;
				int pp = (k-1)/DISK_PAGE_SIZE;
				int ci = k % DISK_PAGE_SIZE;
				int pi = (k-1) % DISK_PAGE_SIZE;
				int a = memory.getVal(frames[cp], ci);
				int b = memory.getVal(frames[pp], pi);
				if (b > a) {
					int temp = b;
					memory.setVal(frames[pp], pi, a);
					memory.setVal(frames[cp], ci, temp);					
				}
			}
		}
		for (int j = 0; j < r-i; j++) {
			memory.writeFrame(inputFile, frames[j], i+j);
			memory.freeFrame(frames[j]);
		}
	}
	runSize = b;
	totalPass = 1;
	totalRuns = inputFile.totalPages/b;
	if (inputFile.totalPages % b > 0) totalRuns++;
	cout << "First Pass Performed" << endl;
	// inputFile.writeDiskFile(); //print file to cout
	// cout << inputFile.totalPages << endl;
	int z = inputFile.totalPages;
	// DiskFile* tempFile = new DiskFile(z);
    DiskFile tempFile(z);
	// cout << tempFile.totalPages << endl;
	tempFile.DiskFileCopy(inputFile, 0, inputFile.totalPages-1);
	this->preserve.push_back(tempFile);
	// tempFile.writeDiskFile();

}

//Performs merging of B-1 runs of runSize each
void ExtMergeSort :: merge(DiskFile &inputFile, MainMemory &memory, int leftStart, int runSize, int k){
	// int k = this->k;
	vector<int> runs, runsEnd;
	int rightEnd = min(leftStart + k*runSize, inputFile.totalPages);

	for (int i = leftStart; i < rightEnd; i += runSize) {
		runs.push_back(i);
		runsEnd.push_back(min(i+runSize, rightEnd));
	}
	assert(runs.size() <= k);
	DiskFile tempFile(rightEnd-leftStart);

	vector<int> frames, indexes;
	for (int i = 0; i < runs.size(); i++) {
		int f = memory.loadPage(inputFile, runs[i]);
		if (f == -1) {
			cout << "Can't proceed further due to unavailability of memory or invalid Page access" << endl;
			exit(1); 
		}
		frames.push_back(f);
		indexes.push_back(0);

	}
	for (int i = 0; i < runs.size(); i++) {
		cout << runs[i] << ", " << runsEnd[i] << ", " << frames[i] << ", " << indexes[i] << "\t";
	}
	cout << endl;
	int resFrame = memory.getEmptyFrame();	//frame to store result
	int resIndex = 0, currPage = 0;
	bool flag = false; // is there a pending number to be written
	bool over;
	while (!over) {
		// one loop through each of the k pages (of each run) and select min as x
		int x = INT_MAX;
		int f = -1;
		over = true;
		flag = false;
		for (int i = 0; i < runs.size(); i++) {
			int index = indexes[i];
			if (index == memory.getValidEntries(frames[i]) && runs[i] < runsEnd[i]-1) { // fetch new page
				memory.freeFrame(frames[i]);
				runs[i] += 1;
				frames[i] = memory.loadPage(inputFile, runs[i]);
				indexes[i] = 0;	
				index = 0;			
			}
			if (index < memory.getValidEntries(frames[i])) {
				over = false;
				flag = true;
				int c = memory.getVal(frames[i], index);
				if (x > c) {
					x = c;
					f = i;
				}
			}
		}
		if (flag) {
			memory.setVal(resFrame, resIndex, x);
			// cout << x << endl;
			indexes[f]++;
			resIndex++;
			if(resIndex == MEM_FRAME_SIZE){
				memory.writeFrame(tempFile, resFrame, currPage);
				memory.freeFrame(resFrame);
				resFrame = memory.getEmptyFrame();
				currPage++; 
				resIndex = 0;
			}
		}
	}
	if (resIndex > 0) {
		memory.writeFrame(tempFile, resFrame, currPage);
	}
	for (int i = 0; i < runs.size(); i++) {
		memory.freeFrame(frames[i]);
	}
	memory.freeFrame(resFrame);
	inputFile.DiskFileCopy(tempFile, leftStart, rightEnd);
}

//Performs k way merge sort on inputFile using memory
void ExtMergeSort :: kWaySort(DiskFile &inputFile, MainMemory &memory){
	// if(memory.totalFrames < this->b)
		// cout << "Error: B-1 way merge sort requires atleast B frames" << endl; 
	
	this->firstPass(inputFile, memory);
	int b = memory.totalFrames;
	int leftStart;
	int k = (b-this->k)/this->k;	
	for(this->runSize = b; this->runSize < inputFile.totalPages; this->runSize *= k){
		cout << "runSize: " << this->runSize << endl;
		for(leftStart = 0; leftStart < inputFile.totalPages-1; leftStart += k*this->runSize){
			cout << "calling " << k << "-way merge for < " << leftStart  << " >" << endl;
			this->merge(inputFile, memory, leftStart, this->runSize, k);
		}
		// inputFile.writeDiskFile();
		int z = inputFile.totalPages;
		DiskFile tempFile(z);
		tempFile.DiskFileCopy(inputFile, 0, inputFile.totalPages-1);
		this->preserve.push_back(tempFile);
		// tempFile.writeDiskFile();
		totalPass++;
	}

	cout << "Total Passes required: " << totalPass << endl;
}

int main()
{

	int page_size;
	cin >> page_size;
    DISK_PAGE_SIZE = page_size;
    MEM_FRAME_SIZE = page_size;

	//reads size of main memory in terms of number of frames available
    int d;
    cin >> d;
	//create main memory of x frames
	MainMemory mm(d);

	int k;
	cin >> k;

	//create a file by taking input from cin
	DiskFile f;
	f.readDiskFile();
	// f.writeDiskFile();


	ExtMergeSort e(k);
	
	//call 2 way externalmerge sort
	e.kWaySort(f,mm);

	//output file by using cout
	f.writeDiskFile(); 
    int pass, page;
    while (1) {
        cin >> pass;
        if (pass == -1) break;
        cin >> page;
        e.print(pass, page);
    }
    return 0;
}