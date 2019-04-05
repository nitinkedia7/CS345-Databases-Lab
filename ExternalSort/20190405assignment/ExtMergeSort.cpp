#include "common.h"
#include "ExtMergeSort.h"

//creates initial runs of B page size
void ExtMergeSort :: firstPass(DiskFile &inputFile, MainMemory &memory){
	int c;
	for (int i = 0; i < inputFile.totalPages; i += this->b) {
		c = 0;
		int r = min(inputFile.totalPages, i+this->b); // [i,r)
		vector<int> frames(this->b, -1);
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
	inputFile.writeDiskFile(); //print file to cout
}

//Performs merging of B-1 runs of runSize each
void ExtMergeSort :: merge(DiskFile &inputFile, MainMemory &memory, int leftStart, int runSize){
	int k = this->b - 1;
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
	if(memory.totalFrames < this->b)
		cout << "Error: B-1 way merge sort requires atleast B frames" << endl; 
	
	this->firstPass(inputFile, memory);

	int leftStart;
	int k = this->b - 1;	
	for(this->runSize = this->b; this->runSize < inputFile.totalPages; this->runSize *= k){
		cout << "runSize: " << this->runSize << endl;
		for(leftStart = 0; leftStart < inputFile.totalPages-1; leftStart += k*this->runSize){
			cout << "calling " << k << "-way merge for < " << leftStart  << " >" << endl;
			this->merge(inputFile, memory, leftStart, this->runSize);
		}
		// inputFile.writeDiskFile();
		totalPass++;
	}

	cout << "Total Passes required: " << totalPass << endl;
}