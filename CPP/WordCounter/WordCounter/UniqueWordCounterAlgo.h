#pragma once
#include <string>

class UniqueWordCounterAlgo {
public:
	virtual ~UniqueWordCounterAlgo() = default;
	virtual const int countUniqueWordsInFile(const std::string file_path) = 0;
};