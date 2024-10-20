#pragma once
#include <mutex>
#include <unordered_set>
#include <thread>
#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include "UniqueWordCounterAlgo.h"

class UniqueWordCounterAlgoImpl:public UniqueWordCounterAlgo {
private:
	const int _maxThreadCount;
	const size_t _blockSize;
	std::mutex _mutexUniqueWords;
	std::unordered_set<std::string> _uniqueWords;
	int _lastRunnedThreads;
public:
	UniqueWordCounterAlgoImpl(size_t blockInKb = 42) :
		_blockSize(blockInKb * 1024),
		_maxThreadCount(std::thread::hardware_concurrency()),
		_lastRunnedThreads(0){}
	~UniqueWordCounterAlgoImpl() = default;
	const int getLastCountOfThreads() const;
	const bool wordExist(const std::string str) const;
	const int countUniqueWordsInFile(const std::string file_path);
private:
	std::streamsize adjustStartPosition(std::unique_ptr<std::ifstream>  file, const  std::streamsize& start) const;
	inline int getCurretnThreadDemand(const  std::streamsize& fileSize)  const;
	void countUniqueWordsOnRange(std::unique_ptr<std::ifstream>  file, const std::streamsize start, const std::streamsize end);
	inline void countUniqueWordsProcedure(const std::string& file_path, const int threadCount, const std::streamsize& fileSize, const std::streamsize& chunkSizeForThread);
};