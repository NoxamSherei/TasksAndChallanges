#pragma once
#include <mutex>
#include <unordered_set>
#include <thread>
#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include "UniqueWordCounterAlgo.h"

using std::vector;
using std::ifstream;
using std::unordered_set;
using std::mutex;
using std::lock_guard;
using std::thread;
using std::string;
using std::streamsize;

class UniqueWordCounterAlgoImpl:public UniqueWordCounterAlgo {
private:
	const int maxThreadCount;
	const size_t blockSize;
	mutex mutexUniqueWords;
	unordered_set<string> uniqueWords;
private:
	streamsize adjustStartPosition(std::unique_ptr<ifstream>  file, streamsize start);
	inline int getCurretnThreadDemand(streamsize fileSize);
	void readThread(std::unique_ptr<ifstream>  file, streamsize start, streamsize end);
public:
	int lastRunnedThreads;
public:
	UniqueWordCounterAlgoImpl(size_t blockInKb = 42) :
		blockSize(blockInKb * 1024),
		maxThreadCount(thread::hardware_concurrency()),
		lastRunnedThreads(0){}
	~UniqueWordCounterAlgoImpl() = default;
	bool wordExist(string str);
	int countUniqueWordsInFile(string file_path);
};