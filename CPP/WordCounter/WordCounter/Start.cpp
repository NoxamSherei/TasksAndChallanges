#include "UniqueWordCounterAlgoImpl.h"

#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
	if (argc != 2) {
		std::cerr << "Missing Arguments. <FilePath>" << std::endl;
		return -1;
	}

	std::string file_path = argv[1];

	UniqueWordCounterAlgo* algo=new UniqueWordCounterAlgoImpl();
	std::cout << algo->countUniqueWordsInFile(file_path) << std::endl;
	delete algo;

	return 0;
}