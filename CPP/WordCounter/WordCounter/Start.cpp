#include "UniqueWordCounterAlgoImpl.h"

#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
	if (argc != 2) {
		std::cerr << "Missing Argument: <FilePath>" << std::endl;
		return -1;
	}

	std::string file_path = argv[1];

	UniqueWordCounterAlgo* algo=new UniqueWordCounterAlgoImpl();
	int uniqueWords = algo->countUniqueWordsInFile(file_path);
	delete algo;
	std::cout << uniqueWords << std::endl;

	return 0;
}