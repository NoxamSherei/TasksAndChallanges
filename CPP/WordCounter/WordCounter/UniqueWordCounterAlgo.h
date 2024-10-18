#pragma once
#include <string>

using std::string;

class UniqueWordCounterAlgo {
public:
	virtual ~UniqueWordCounterAlgo() = default;
	virtual int countUniqueWordsInFile(string file_path) = 0;
};