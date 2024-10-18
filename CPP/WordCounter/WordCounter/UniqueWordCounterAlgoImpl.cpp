#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <thread>

#include "UniqueWordCounterAlgoImpl.h"

using std::vector;
using std::ifstream;
using std::istringstream;
using std::thread;
using std::exception;
using std::cerr;
using std::cout;
using std::endl;

void UniqueWordCounterAlgoImpl::countUniqueWordsInLine(string line) {
	unordered_set<string> uniqueWordsInLine;
	istringstream sstream(line);
	string word;

	while (sstream >> word) {
		uniqueWordsInLine.insert(word);
	}

	lock_guard<mutex> lock(mutex_uniqueWords);
	uniqueWords.insert(uniqueWordsInLine.begin(), uniqueWordsInLine.end());
}

int UniqueWordCounterAlgoImpl::countUniqueWordsInFile(string file_path) {
	ifstream file(file_path);
	if (!file) {
		cerr << "Missing file:" << file_path << endl;
		return -1;
	}

	try {
		string line;
		vector<thread> threads;
		while (getline(file, line)) {
			if (line.empty()) {
				continue;
			}
			threads.emplace_back(&UniqueWordCounterAlgoImpl::countUniqueWordsInLine, this, line);
		}
		file.close();

		for (auto& singleThread : threads) {
			singleThread.join();
		}
		return uniqueWords.size();
	}
	catch (exception e) {
		cout << e.what() << endl;
	}

	if (file.is_open()) {
		file.close();
	}

	return 0;
}