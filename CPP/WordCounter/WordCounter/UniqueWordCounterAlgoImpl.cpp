#include "UniqueWordCounterAlgoImpl.h"

bool UniqueWordCounterAlgoImpl::wordExist(string str) {
	return uniqueWords.find(str) != uniqueWords.end();
}

streamsize UniqueWordCounterAlgoImpl::adjustStartPosition(std::unique_ptr<ifstream>  file, streamsize start) {
	file->seekg(start);
	char current;
	while (file->get(current)) {
		if (current == ' ' || current == '\n') {
			return file->tellg();
		}
	}
	return start;
}

inline int UniqueWordCounterAlgoImpl::getCurretnThreadDemand(streamsize fileSize) {
	return std::max(1, std::min(static_cast<int>(fileSize / blockSize), maxThreadCount));
}

int UniqueWordCounterAlgoImpl::countUniqueWordsInFile(string file_path) {
	uniqueWords.clear();
	ifstream file(file_path, std::ios::binary | std::ios::ate);
	if (!file) {
		std::cerr << "Missing file:" << file_path << std::endl;
		return -1;
	}
	streamsize fileSize = file.tellg();
	file.close();
	int threadCount = getCurretnThreadDemand(fileSize);
	streamsize chunkSizeForThread = fileSize / threadCount;

	vector<thread> threads;
	streamsize start = 0;
	for (int i = 0; i < threadCount; ++i) {
		streamsize end = (i == threadCount - 1) ? fileSize :
			adjustStartPosition(std::move(std::make_unique<ifstream>(file_path)), start + chunkSizeForThread);

		threads.emplace_back(&UniqueWordCounterAlgoImpl::readThread, this, std::move(std::make_unique<ifstream>(file_path)), start, end);
		start = end;

		if (end >= fileSize) {
			break;
		}
	}
	lastRunnedThreads = threads.size();
	for (auto& singleThread : threads) {
		singleThread.join();
	}
	return uniqueWords.size();
}

void UniqueWordCounterAlgoImpl::readThread(std::unique_ptr<ifstream>  file, streamsize start, streamsize end) {
	unordered_set<string> uniqueWordsInLine;
	string word = "";
	char current;
	file->seekg(start);
	while (file->get(current) && file->tellg() <= end) {
		if (!std::isalpha(current) && current != '\'') {
			if (word.empty()) {
				continue;
			}
			uniqueWordsInLine.insert(word);
			word.clear();
			continue;
		}
		word += current;
	}
	if (!word.empty()) {
		uniqueWordsInLine.insert(word);
	}
	lock_guard<mutex> lock(mutexUniqueWords);
	uniqueWords.insert(uniqueWordsInLine.begin(), uniqueWordsInLine.end());
	file->close();
}