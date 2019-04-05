#pragma once
#include "common.h"
#include "DiskFile.h"
#include "Page.h"
#include "Frame.h"
#include "MainMemory.h"

class ExtMergeSort{
public:
	int runSize; // size of run in terms of number of pages
	int totalPass; // number of passes performed
	int totalRuns;
	int b;

	ExtMergeSort(int b){
		runSize = 0;
		totalPass = 0;
		totalRuns = -1;
		this->b = b;
	}

	void firstPass(DiskFile &inputFile, MainMemory &memory);
	void merge(DiskFile &inputFile, MainMemory &memory, int leftStart, int runSize);
	void kWaySort(DiskFile &inputFile, MainMemory &memory);
};