#include "common.h"
#include "ExtMergeSort.h"

//creates initial runs of B page size
void ExtMergeSort :: firstPass(DiskFile &inputFile, MainMemory &memory){
	int c = 0;
	for (int i = 0; i < inputFile.totalPages; i += this->b) {
		int r = min(inputFile.totalPages, i+this->b); // [i,r)
		vector<int> frames(this->b, -1);
		for (int j = i; j < r; j++) {
			frames[j-i] = memory.loadPage(inputFile, j);
			c += memory.data[frames[j-i]].validEntries;
		}
		// apply bubblesort to B pages
		for (; c > 0; c--) {
			int max = memory.getVal(frames[0], 0);
			for (int j = 0; j < r-i; j++) {
				max = memory.bubbleSortPass(frames[j], max);	
			}
		}
		for (int j = 0; j < r-i; j++) {
			memory.writeFrame(inputFile, frames[j], i+j);
			memory.freeFrame(frames[j]);
		}
	}	
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
	int resFrame = memory.getEmptyFrame();	//frame to store result
	int resIndex = 0, currPage = 0;
	bool flag = false; // is there a pending number to be written
	bool over;
	while (!over) {
		// one loop through each of the k pages (of each run) and select min as x
		int x = INT_MAX;
		int f = -1;
		over = true;
		for (int i = 0; i < runs.size(); i++) {
			int index = indexes[i];
			if (index == memory.getValidEntries(frames[i]) && runs[i] < runsEnd[i]) { // fetch new page
				over = false;
				memory.freeFrame(runs[i]);
				runs[i] += 1;
				frames[i] = memory.loadPage(inputFile, runs[i]);
				indexes[i] = 0;	
				index = 0;			
			}
			if (index < memory.getValidEntries(frames[i])) {
				over = false;
				int c = memory.getVal(frames[i], index);
				if (x > c) {
					x = c;
					f = i;
				}
			}
		}
		if (!over) {
			memory.setVal(resFrame, resIndex, x);
			flag = true;
			indexes[f]++;
			resIndex++;
			if(resIndex == MEM_FRAME_SIZE){
				memory.writeFrame(tempFile, resFrame, currPage);
				flag = false;
				memory.freeFrame(resFrame);
				resFrame = memory.getEmptyFrame();
				currPage++; 
				resIndex = 0;
			}
		}
	}
	for (int i = 0; i < runs.size(); i++) {
		memory.freeFrame(frames[i]);
	}
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
		for(leftStart = 0; leftStart < inputFile.totalPages-1; leftStart += 2*this->runSize){
			cout << "calling " << k << "-way merge for < " << leftStart  << " > with runsize " << this->runSize << endl;
			this->merge(inputFile, memory, leftStart, this->runSize);
		}
		totalPass++;
	}

	cout << "Total Passes required: " << totalPass << endl;
}