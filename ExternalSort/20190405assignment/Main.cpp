#include "common.h"
#include "MainMemory.cpp"
#include "DiskFile.cpp"
#include "ExtMergeSort.cpp"
#include "Frame.cpp"
#include "Page.cpp"

using namespace std;

int main()
{
	int x;
	//reads size of main memory in terms of number of frames available
	cin >> x;

	//create main memory of x frames
	MainMemory mm(x);

	int b;
	cin >> b;

	//create a file by taking input from cin
	DiskFile f;
	f.readDiskFile();
	f.writeDiskFile();

	ExtMergeSort e(b);
	
	//call 2 way externalmerge sort
	e.kWaySort(f,mm);

	//output file by using cout
	f.writeDiskFile(); 

}