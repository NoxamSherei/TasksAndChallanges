#pragma once
#include <mutex>
#include <unordered_set>
#include "UniqueWordCounterAlgo.h"

using std::unordered_set;
using std::mutex;
using std::lock_guard;

class UniqueWordCounterAlgoImpl : public UniqueWordCounterAlgo {
private:
	mutex mutex_uniqueWords;
	unordered_set<string> uniqueWords;
private:
	void countUniqueWordsInLine(string line);
public:
	UniqueWordCounterAlgoImpl() = default;
	~UniqueWordCounterAlgoImpl() = default;
	int countUniqueWordsInFile(string file_path) override;
};