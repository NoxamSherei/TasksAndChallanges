#include "UniqueWordCounterAlgoImpl.h"

const bool UniqueWordCounterAlgoImpl::wordExist(const std::string str) const {
	return _uniqueWords.find(str) != _uniqueWords.end();
}

const int UniqueWordCounterAlgoImpl::getLastCountOfThreads() const {
	return _lastRunnedThreads;
}

inline int UniqueWordCounterAlgoImpl::getCurretnThreadDemand(const  std::streamsize& fileSize) const {
	return std::max(1, std::min(static_cast<int>(fileSize / _blockSize), _maxThreadCount));
}

inline bool isWhitespaceOrNewline(char currentChar){
	return currentChar == ' ' || currentChar == '\n';
}

inline bool isPartOfWord(char currentChar) {
	return (currentChar >= 'a' && currentChar <= 'z') || (currentChar >= 'A' && currentChar <= 'Z') || currentChar == '\'';
}

std::streamsize UniqueWordCounterAlgoImpl::adjustStartPosition(std::unique_ptr<std::ifstream>  file, const std::streamsize& start) const {
	file->seekg(start);
	char currentChar;
	while (file->get(currentChar)) {
		if (!isWhitespaceOrNewline(currentChar)) {
			continue;
		}
		return file->tellg();
	}
	return start;
}

const int UniqueWordCounterAlgoImpl::countUniqueWordsInFile(const std::string file_path) {
	_uniqueWords.clear();
	std::ifstream file(file_path, std::ios::binary | std::ios::ate);
	if (!file) {
		std::cerr << "Missing file:" << file_path << std::endl;
		return -1;
	}
	std::streamsize fileSize = file.tellg();
	file.close();
	int threadCount = getCurretnThreadDemand(fileSize);
	std::streamsize chunkSizeForThread = fileSize / threadCount;

	countUniqueWordsProcedure(file_path, threadCount, fileSize, chunkSizeForThread);
	return _uniqueWords.size();
}

inline void UniqueWordCounterAlgoImpl::countUniqueWordsProcedure(
	const std::string& file_path,
	const int threadCount,
	const std::streamsize& fileSize,
	const std::streamsize& chunkSizeForThread) {
	std::vector<std::thread> threads;
	std::streamsize from = 0;
	for (int i = 0; i < threadCount; ++i) {
		std::streamsize to = (i == threadCount - 1) ? fileSize : adjustStartPosition(std::make_unique<std::ifstream>(file_path), from + chunkSizeForThread);

		threads.emplace_back(&UniqueWordCounterAlgoImpl::countUniqueWordsOnRange, this, std::make_unique<std::ifstream>(file_path), from, to);
		from = to;

		if (to >= fileSize) {
			break;
		}
	}
	_lastRunnedThreads = threads.size();
	for (auto& singleThread : threads) {
		singleThread.join();
	}
}

void UniqueWordCounterAlgoImpl::countUniqueWordsOnRange(std::unique_ptr<std::ifstream> fileStream, const std::streamsize from, const std::streamsize to) {
	std::unordered_set<std::string> _uniqueWordsInChunk;
	std::string word = "";
	char currentChar;
	fileStream->seekg(from);
	while (fileStream->get(currentChar) && fileStream->tellg() <= to) {
		if (isPartOfWord(currentChar)) {
			word += currentChar;
			continue;
		}

		if (word.empty()) {
			continue;
		}
		_uniqueWordsInChunk.insert(word);
		word.clear();
	}
	if (!word.empty()) {
		_uniqueWordsInChunk.insert(word);
	}
	std::lock_guard<std::mutex> lock(_mutexUniqueWords);
	_uniqueWords.insert(_uniqueWordsInChunk.begin(), _uniqueWordsInChunk.end());
	fileStream->close();
}